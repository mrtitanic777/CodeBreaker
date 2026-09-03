; dlc_lcode_01_01.asm -- DLC object 1 segment _LTEXT  (part 1/9)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  05Ah, 059h, 0CDh, 020h, 01Fh, 000h, 001h, 000h, 0FFh, 0E2h, 0CDh, 020h, 0F4h, 080h, 001h, 000h
        db  08Dh, 044h, 024h, 008h, 050h, 0FFh, 074h, 024h, 008h, 0CDh, 020h, 02Dh, 001h, 001h, 000h, 083h
        db  0C4h, 008h, 0C3h
sub_1_0023:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_1_002E:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
sub_1_0034:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
sub_1_003A:
        db  0CDh, 020h, 063h, 080h, 001h, 000h
sub_1_0040:
        db  0CDh, 020h, 064h, 080h, 001h, 000h
sub_1_0046:
        db  0CDh, 020h, 067h, 080h, 001h, 000h
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jae    SHORT loc_1_0061
        xor    eax, eax
        jmp    SHORT loc_1_006A
loc_1_0061:
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [esi], edx
        movzx  eax, cx
loc_1_006A:
        pop    esi
        ret    0ch
        db  053h, 08Bh, 05Ch, 024h, 008h, 0CDh, 020h, 02Ch, 000h, 001h, 000h, 073h, 004h, 033h, 0C0h, 0EBh
        db  002h, 08Bh, 0C3h, 05Bh, 0C2h, 004h, 000h, 0CDh, 020h, 06Eh, 080h, 001h, 000h, 056h, 08Bh, 054h
        db  024h, 010h, 08Bh, 044h, 024h, 00Ch, 08Bh, 074h, 024h, 008h, 0CDh, 020h, 0FEh, 000h, 001h, 000h
        db  08Bh, 0C6h, 05Eh, 0C2h, 00Ch, 000h, 053h, 08Bh, 05Ch, 024h, 008h, 0CDh, 020h, 02Bh, 000h, 001h
        db  000h, 073h, 004h, 033h, 0C0h, 0EBh, 002h, 08Bh, 0C3h, 05Bh, 0C2h, 004h, 000h
sub_1_00BB:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_1_00CA:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        jmp    edx
        db  08Bh, 05Ch, 024h, 004h, 08Bh, 044h, 024h, 004h, 0CDh, 020h, 001h, 000h, 02Ah, 000h, 0C2h, 004h
        db  000h
sub_1_00E5:
        db  0CDh, 020h, 00Eh, 080h, 02Ah, 000h
sub_1_00EB:
        db  0CDh, 020h, 014h, 080h, 02Ah, 000h
sub_1_00F1:
        pop    edx
        pop    eax
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        jmp    edx
sub_1_00FB:
        pop    edx
        pop    eax
        pop    ecx
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        jmp    edx
sub_1_0106:
        db  0CDh, 020h, 00Eh, 080h, 017h, 000h
        push   esi
        mov    ebx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 18h]
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jae    SHORT loc_1_012D
        xor    eax, eax
        jmp    SHORT loc_1_0133
loc_1_012D:
        or     esi, esi
        jne    SHORT loc_1_0133
        mov    eax, ecx
loc_1_0133:
        pop    esi
        ret    14h
sub_1_0137:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
        or     eax, 1000h
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        pop    edi
        pop    ebx
        ret    10h
sub_1_0159:
        xor    eax, eax
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jae    SHORT loc_1_0165
        xor    eax, eax
loc_1_0165:
        ret
sub_1_0166:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
sub_1_017F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
sub_1_018E:
        db  0CDh, 020h, 053h, 080h, 001h, 000h
sub_1_0194:
        db  0CDh, 020h, 055h, 080h, 001h, 000h
sub_1_019A:
        db  0CDh, 020h, 001h, 081h, 001h, 000h
sub_1_01A0:
        db  0CDh, 020h, 002h, 081h, 001h, 000h
sub_1_01A6:
        db  0CDh, 020h, 02Eh, 081h, 001h, 000h
sub_1_01AC:
        db  0CDh, 020h, 02Fh, 081h, 001h, 000h
sub_1_01B2:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_1_01BD:
        db  0CDh, 020h, 057h, 080h, 001h, 000h
        db  0CDh, 020h, 002h, 080h, 033h, 000h
sub_1_01C9:
        db  0CDh, 020h, 003h, 080h, 033h, 000h
sub_1_01CF:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
sub_1_01D5:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_01E0:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        db  0CDh, 020h, 0FCh, 000h, 001h, 000h
        pop    esi
        pop    ebx
        ret    8
sub_1_01F5:
        push   edi
        push   esi
        push   edx
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 18h]
        db  0CDh, 020h, 0FBh, 000h, 001h, 000h
        mov    eax, esi
        pop    edx
        pop    esi
        pop    edi
        ret    0ch
sub_1_0212:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        pop    esi
        ret    4
sub_1_0221:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 28h]
        mov    edi, dword ptr [esp + 2ch]
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    18h
        db  0B8h, 012h, 000h, 000h, 000h, 0CDh, 020h, 093h, 000h, 001h, 000h, 0C3h, 056h, 08Bh, 044h, 024h
        db  008h, 08Bh, 074h, 024h, 00Ch, 0CDh, 020h, 041h, 000h, 001h, 000h, 073h, 004h, 033h, 0C0h, 0EBh
        db  004h, 08Bh, 044h, 024h, 008h, 05Eh, 0C2h, 008h, 000h, 08Bh, 044h, 024h, 004h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 0C2h, 004h, 000h
DLC_Control:
        cmp    eax, 1bh
        jne    SHORT loc_1_0292
        db  0E8h, 072h, 0FDh, 0FBh, 0FFh
        cmp    eax, 1
        ret
loc_1_0292:
        cmp    eax, 1ch
        jne    SHORT loc_1_02A0
        call   sub_1_B1EC
        cmp    eax, 1
        ret
loc_1_02A0:
        cmp    eax, 23h
        jne    SHORT loc_1_02B2
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_AE18
        cmp    eax, 1
        ret
loc_1_02B2:
        cmp    eax, 0bh
        jne    SHORT loc_1_02C4
        push   ebx
        call   sub_1_28CA4
        add    esp, 4
        cmp    eax, 1
        ret
loc_1_02C4:
        cmp    eax, 14h
        jne    SHORT loc_1_02D2
        call   sub_1_B1D6
        cmp    eax, 1
        ret
loc_1_02D2:
        clc
        ret
        db  066h, 09Ch, 066h, 058h, 0FAh, 0C3h
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        test   ax, 200h
        je     SHORT loc_1_02EA
        sti
        jmp    SHORT loc_1_02EB
loc_1_02EA:
        cli
loc_1_02EB:
        pop    ebp
        ret
sub_1_02ED:
        mov    edx, dword ptr [esp + 4]
        db  00Fh, 0B7h, 004h, 055h, 00Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        ret
sub_1_0300:
        pop    edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        push   edi
        ret
sub_1_0312:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        mov    eax, dword ptr [eax + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        pop    ebx
        ret
sub_1_0349:
        pop    esi
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        push   esi
        ret
loc_1_035B:
        xor    eax, eax
        ret
sub_1_035E:
        push   edi
        mov    edi, 0f0000h
        mov    al, 3
        mov    ecx, 0fff0h
        repne scasb
        mov    eax, edi
        pop    edi
        jne    SHORT loc_1_035B
        dec    eax
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    word ptr [ebx + 1ch], 0aaaah
        mov    word ptr [ebx + 38h], 0f000h
        mov    word ptr [ebx + 10h], ax
        mov    eax, 5ch
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    eax, eax
        mov    cx, word ptr [ebx + 1ch]
        or     cx, cx
        jne    SHORT loc_1_03C3
        movzx  eax, word ptr [ebx + 38h]
        shl    eax, 10h
        mov    ax, word ptr [ebx + 10h]
loc_1_03C3:
        pop    ebx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h, 0C3h, 08Bh, 04Ch, 024h, 004h, 08Bh, 054h, 024h, 008h, 053h
        db  057h, 056h, 0B8h, 000h, 010h, 000h, 000h, 08Bh, 0D9h, 0B9h, 001h, 000h, 000h, 000h, 0BEh, 09Bh
        db  077h, 002h, 000h, 0CDh, 020h, 05Ah, 001h, 001h, 000h, 05Eh, 05Fh, 05Bh, 0C3h
sub_1_0407:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        push   ebx
        push   edi
        push   esi
        mov    esi, eax
        mov    eax, 1000h
        mov    ebx, ecx
        mov    ecx, 1
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    edi
        pop    ebx
        ret
sub_1_0430:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        call   sub_1_0447
        pop    ebp
        pop    ebx
        ret
sub_1_0447:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, edx
        movzx  edx, dx
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 10h], dx
        shl    eax, 4
        add    eax, edx
        movzx  cx, byte ptr [eax + 2]
        mov    word ptr [ebp + 1ch], cx
        mov    cl, byte ptr [eax]
        mov    word ptr [ebp + 18h], cx
        mov    ecx, dword ptr [eax + 8]
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, dword ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
        db  08Bh, 04Ch, 024h, 004h, 08Bh, 044h, 024h, 008h, 08Bh, 054h, 024h, 00Ch, 053h, 057h, 056h, 08Bh
        db  0F0h, 0B8h, 000h, 010h, 000h, 000h, 08Bh, 0D9h, 0B9h, 001h, 000h, 000h, 000h, 0CDh, 020h, 05Ah
        db  001h, 001h, 000h, 05Eh, 05Fh, 05Bh, 0C3h
sub_1_04E7:
        call   sub_1_035E
        or     eax, eax
        je     SHORT loc_1_0509
        db  0A3h, 038h, 090h, 002h, 000h
        call   sub_1_063A
        jae    SHORT loc_1_04FD
        ret
loc_1_04FD:
        call   sub_1_28A2A
        or     eax, eax
        jne    SHORT loc_1_0509
        xor    eax, eax
        ret
loc_1_0509:
        call   sub_1_066B
        xor    eax, eax
        inc    eax
        ret
sub_1_0512:
        call   sub_1_066B
        call   sub_1_28B01
        ret
sub_1_051D:
        push   edi
        xor    eax, eax
        mov    edi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, edi
        neg    ecx
        and    ecx, 3
        cmp    edx, ecx
        jb     SHORT loc_1_0541
        sub    edx, ecx
        rep stosb
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        and    edx, 3
loc_1_0541:
        mov    ecx, edx
        rep stosb
        pop    edi
        ret
sub_1_0547:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        rep stosb
        pop    edi
        ret
sub_1_0558:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        rep movsb
        pop    edi
        pop    esi
        ret
loc_1_056B:
        movzx  eax, word ptr [ebp + 38h]
        mov    ecx, eax
        rol    ecx, 4
        movzx  edx, word ptr [ebp + 10h]
        add    ecx, edx
        cmp    byte ptr [ecx], 1
        ja     SHORT loc_1_059F
        shl    eax, 10h
        add    eax, edx
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 0ffh
        push   eax
        call   sub_1_287DD
        pop    eax
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        clc
        ret
loc_1_059F:
        stc
        ret
sub_1_05A1:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
sub_1_05AC:
        db  0CDh, 020h, 00Dh, 000h, 02Ah, 000h
        ret
sub_1_05B3:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_05BE:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_05CF:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esp + 8]
        push   1
        push   ecx
        push   eax
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_05F1
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        ret
loc_1_05F1:
        ret
sub_1_05F2:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_0603:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        push   0
        push   0
        push   0
        push   0
        push   0
        push   eax
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
        ret
sub_1_0629:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        ret
sub_1_063A:
        mov    eax, 5ch
        db  0BEh, 06Bh, 005h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        ret
sub_1_064B:
        push   ebx
        push   edi
        push   esi
        db  0BEh, 093h, 089h, 002h, 000h
        mov    eax, 1000h
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    eax, esi
        pop    esi
        pop    edi
        pop    ebx
        ret
sub_1_066B:
        db  083h, 03Dh, 034h, 090h, 002h, 000h, 000h
        jne    SHORT loc_1_0675
        ret
loc_1_0675:
        mov    eax, 5ch
        db  0BEh, 06Bh, 005h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        db  0FFh, 00Dh, 034h, 090h, 002h, 000h
        ret
sub_1_068C:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        ret
sub_1_0693:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
sub_1_0699:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [ebp - 75h], dl
        in     al, dx
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_06E9
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_07F6
loc_1_06E9:
        push   10h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0716
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_0734
loc_1_0716:
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_07F6
loc_1_0734:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 0
        je     NEAR PTR loc_1_0781
        push   2
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_07F6
loc_1_0781:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_0F60
        add    esp, 10h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_07CB
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [eax + 8], 0
loc_1_07CB:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_07F6
loc_1_07F6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_07FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_0844
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_09A6
loc_1_0844:
        push   0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0871
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_088F
loc_1_0871:
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_09A6
loc_1_088F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_08E5
        push   0a2h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_09A6
loc_1_08E5:
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_1_091D
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 2], ax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_09A6
loc_1_091D:
        push   1
        push   0
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_097B
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 2], ax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_09A6
        db  0E9h, 02Bh, 000h, 000h, 000h
loc_1_097B:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 2], ax
        push   1ch
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_09A6
loc_1_09A6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_09AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_09F4
        mov    al, byte ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_0B67
loc_1_09F4:
        push   0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0A21
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_0A3F
loc_1_0A21:
        push   1bh
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_0B67
loc_1_0A3F:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0A95
        push   0a2h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_0B67
loc_1_0A95:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_0AD9
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_21D8
        add    esp, 1ch
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_0AFC
loc_1_0AD9:
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax + 6]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_249F
        add    esp, 18h
        mov    dword ptr [ebp - 18h], eax
loc_1_0AFC:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_0B10
        cmp    dword ptr [ebp - 18h], 3
        jne    NEAR PTR loc_1_0B49
loc_1_0B10:
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 2], ax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_0B67
        db  0E9h, 01Eh, 000h, 000h, 000h
loc_1_0B49:
        push   12h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_0B67
loc_1_0B67:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0B6C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], edi
        mov    dword ptr [ebp - 4], edx
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0B8D
        jmp    NEAR PTR loc_1_0C97
loc_1_0B8D:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0BA6
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 8], ecx
        je     NEAR PTR loc_1_0BC8
loc_1_0BA6:
        push   22h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   1f3h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0C97
loc_1_0BC8:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0BF3
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 24h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_0BF3:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 24h], 0
        je     NEAR PTR loc_1_0C5B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [eax + 14h], ecx
        jae    NEAR PTR loc_1_0C5B
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, dword ptr [ecx + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 24h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_0C48
        mov    eax, ecx
loc_1_0C48:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2908
        add    esp, 8
loc_1_0C5B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 18h]
        cmp    dword ptr [eax + 24h], ecx
        jbe    NEAR PTR loc_1_0C7F
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [eax + 14h], ecx
        jne    NEAR PTR loc_1_0C89
loc_1_0C7F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], 0
loc_1_0C89:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_0C97:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0C9C:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0CBA:
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 34h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_0E03
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 34h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 30h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 30h]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0D19
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_0D19:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0D5A
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 30h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 34h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_0D5A:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0D93
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_0D93:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0DD4
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 28h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_0DD4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        imul   eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        imul   ecx, dword ptr [ebp - 8]
        add    eax, ecx
        sub    edx, edx
        div    dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 34h], eax
        jmp    NEAR PTR loc_1_0E22
loc_1_0E03:
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_0E22
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_0F5B
loc_1_0E22:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_0E41
        mov    eax, ecx
loc_1_0E41:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_0E5C
        mov    eax, ecx
loc_1_0E5C:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 28h], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0E9E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_0E9E:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_0ED6
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 28h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_0ED6:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [eax + 14h], ecx
        jae    NEAR PTR loc_1_0F46
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 18h], ecx
        jae    NEAR PTR loc_1_0F46
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_0F0E
        mov    eax, ecx
loc_1_0F0E:
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 18h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 14h]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_0F30
        mov    eax, ecx
loc_1_0F30:
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2908
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
loc_1_0F46:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_0F5B
loc_1_0F5B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0F60:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_0F8E
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 1ch
        xor    eax, eax
        jmp    NEAR PTR loc_1_1325
loc_1_0F8E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0fffh
        and    eax, 0fffff000h
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        sub    ecx, dword ptr [ebp + 8]
        sub    eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0fffh
        sub    eax, ecx
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 1000h
        jae    NEAR PTR loc_1_102C
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 18h
        xor    eax, eax
        jmp    NEAR PTR loc_1_1325
loc_1_102C:
        mov    eax, dword ptr [ebp - 0ch]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_104A
        mov    eax, ecx
loc_1_104A:
        cmp    eax, 1000h
        ja     NEAR PTR loc_1_105A
        mov    eax, 1000h
loc_1_105A:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        lea    eax, [eax + ecx - 1]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        mov    eax, dword ptr [ebp - 18h]
        lea    eax, [eax*8 - 8]
        add    eax, 1061h
        shr    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        push   81h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_018E
        add    esp, 20h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_10E3
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 12h
        xor    eax, eax
        jmp    NEAR PTR loc_1_1325
loc_1_10E3:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        call   sub_1_01D5
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 2ch], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_115A:
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 30h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_116E:
        mov    ecx, 3e8h
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 34h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 38h], 0
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_1196
loc_1_1192:
        inc    word ptr [ebp - 14h]
loc_1_1196:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, 5
        jge    NEAR PTR loc_1_11E2
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + eax*4 + 3ch], 0ffffh
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + eax*4 + 3eh], 0ffffh
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + eax*2 + 50h], 0
        jmp    NEAR PTR loc_1_1192
loc_1_11E2:
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_11F1
loc_1_11ED:
        inc    word ptr [ebp - 14h]
loc_1_11F1:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 18h]
        jae    NEAR PTR loc_1_125A
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    dword ptr [ecx + eax*8 + 5ah], 0ffh
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    dword ptr [ecx + eax*8 + 5ah], 0fffffff1h
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [ecx + eax*8 + 5ah], 0eh
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    dword ptr [ecx + eax*8 + 5ah], 0fffffffeh
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [ecx + eax*8 + 5ah], 1
        jmp    NEAR PTR loc_1_11ED
loc_1_125A:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_2908
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_130C
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_1292
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_1292:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_12C9
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_12C9:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_12F9
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_12F9:
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1325
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_130C:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_18D9
        add    esp, 4
        mov    eax, dword ptr [ebp + 14h]
        mov    byte ptr [eax], 12h
        xor    eax, eax
        jmp    NEAR PTR loc_1_1325
loc_1_1325:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_132A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1349
loc_1_1341:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_1349:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1392
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_138D
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 38h], 0
        je     NEAR PTR loc_1_138D
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0C9C
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_1397
        add    esp, 4
loc_1_138D:
        jmp    NEAR PTR loc_1_1341
loc_1_1392:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1397:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_13A5:
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 38h], ecx
        jbe    NEAR PTR loc_1_13BC
        jmp    NEAR PTR loc_1_1767
loc_1_13BC:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_13E2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_13E9
loc_1_13E2:
        mov    dword ptr [ebp - 20h], 0
loc_1_13E9:
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    ecx, eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edx, edx
        div    ecx
        mov    dword ptr [ebp - 4], eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        movsx  ecx, word ptr [eax + 10h]
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        movsx  eax, ax
        sub    ecx, eax
        mov    word ptr [ebp - 14h], cx
        jmp    NEAR PTR loc_1_144E
loc_1_1434:
        movsx  ecx, word ptr [ebp - 14h]
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        movsx  eax, ax
        sub    ecx, eax
        mov    word ptr [ebp - 14h], cx
loc_1_144E:
        movsx  eax, word ptr [ebp - 14h]
        test   eax, eax
        jl     NEAR PTR loc_1_15FF
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_15FF
        movsx  eax, word ptr [ebp - 14h]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffff00h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_15FA
        mov    eax, dword ptr [ebp - 1ch]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_15FA
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        mov    ecx, 8
        and    ecx, 0fffffffeh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_15FA
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C54
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 0ch
        mov    dword ptr [ebp - 10h], eax
        push   40000000h
        push   1
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0040
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_156E
        xor    ecx, ecx
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        sub    ecx, eax
        neg    ecx
        mov    eax, dword ptr [ebp + 8]
        sub    dword ptr [eax + 14h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 18h]
        cmp    dword ptr [eax + 14h], ecx
        jae    NEAR PTR loc_1_1569
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   18h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   3e9h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1767
loc_1_1569:
        jmp    NEAR PTR loc_1_1590
loc_1_156E:
        push   0eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   3f3h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1767
loc_1_1590:
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_159F
loc_1_159B:
        inc    word ptr [ebp - 18h]
loc_1_159F:
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        movsx  eax, ax
        movsx  ecx, word ptr [ebp - 18h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_15F7
        movsx  eax, word ptr [ebp - 18h]
        movsx  ecx, word ptr [ebp - 14h]
        add    eax, ecx
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        and    dword ptr [eax], 0ffh
        mov    eax, dword ptr [ebp - 1ch]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp - 1ch]
        or     dword ptr [eax], 0eh
        mov    eax, dword ptr [ebp - 1ch]
        and    dword ptr [eax], 0fffffffeh
        mov    eax, dword ptr [ebp - 1ch]
        or     dword ptr [eax], 1
        jmp    NEAR PTR loc_1_159B
loc_1_15F7:
        dec    dword ptr [ebp - 4]
loc_1_15FA:
        jmp    NEAR PTR loc_1_1434
loc_1_15FF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_1615
        mov    eax, ecx
loc_1_1615:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 20h]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_1631
        mov    eax, ecx
loc_1_1631:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 20h]
        ja     NEAR PTR loc_1_172B
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_1678
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_1678:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_16BD
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_16BD:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_1702
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 28h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_1702:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_172B
        mov    eax, dword ptr [ebp + 8]
        add    eax, 50h
        push   eax
        push   5
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_172B:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_174B
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        jmp    NEAR PTR loc_1_1759
loc_1_174B:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 3e8h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 38h], eax
loc_1_1759:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_1767:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_176C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*4 + 3eh]
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 14h]
        movsx  eax, word ptr [eax]
        cmp    eax, -1
        jne    NEAR PTR loc_1_17BA
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_18D4
loc_1_17BA:
        mov    eax, dword ptr [ebp + 14h]
        movsx  eax, word ptr [eax]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, -1
        jne    NEAR PTR loc_1_1807
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*4 + 3ch], 0ffffh
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*4 + 3eh], 0ffffh
        jmp    NEAR PTR loc_1_182F
loc_1_1807:
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 4], 0ffffh
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*4 + 3eh], ax
loc_1_182F:
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        lea    ecx, [ecx + ecx*2]
        db  02Bh, 004h, 08Dh, 008h, 000h, 000h, 000h
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        dec    word ptr [ecx + eax*2 + 50h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 4], 0ffffh
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + 6], 0ffffh
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        and    dword ptr [eax], 0fffffffeh
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 14h]
        movsx  ecx, word ptr [ecx]
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffff00h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_18CC
        push   17h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   492h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_18CC:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_18D4
loc_1_18D4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_18D9:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_1_1902
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_01E0
loc_1_1902:
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_192E
loc_1_190D:
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    word ptr [ebp - 10h], ax
loc_1_192E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    dword ptr [eax + 10h], ecx
        jbe    NEAR PTR loc_1_19B4
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*8 + 5ah]
        and    eax, 0ffffff00h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_19AF
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 0ch
        mov    dword ptr [ebp - 0ch], eax
        push   40000000h
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0040
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_19AF
        push   0dh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4c8h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_1A7D
loc_1_19AF:
        jmp    NEAR PTR loc_1_190D
loc_1_19B4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        lea    eax, [eax*8 - 8]
        add    eax, 1061h
        shr    eax, 0ch
        mov    dword ptr [ebp - 4], eax
        push   2000h
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_01BD
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1A17
        push   0dh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4d8h
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_1A7D
loc_1_1A17:
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0194
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_1A54
        push   5
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4deh
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_1A7D
loc_1_1A54:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_1A76
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_1A76:
        xor    eax, eax
        jmp    NEAR PTR loc_1_1A7D
loc_1_1A7D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        movsx  eax, word ptr [ebp + 0ch]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [eax], 0ffffff00h
        jne    NEAR PTR loc_1_1AEA
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   17h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   510h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1C4F
loc_1_1AEA:
        mov    eax, dword ptr [ebp - 8]
        and    dword ptr [eax], 0fffffffeh
        mov    eax, dword ptr [ebp - 8]
        or     dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        shr    eax, 1
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + eax*4 + 3ch]
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, -1
        jne    NEAR PTR loc_1_1B3D
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*4 + 3eh], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 4], 0ffffh
        jmp    NEAR PTR loc_1_1B63
loc_1_1B3D:
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
loc_1_1B63:
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*4 + 3ch], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 6], 0ffffh
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 18h]
        cmp    dword ptr [eax + 14h], ecx
        jae    NEAR PTR loc_1_1C36
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        push   eax
        movsx  eax, word ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   18h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   542h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_1C4F
loc_1_1C36:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        inc    word ptr [ecx + eax*2 + 50h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_1C4F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1C54:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        movsx  eax, word ptr [ebp + 0ch]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        shr    eax, 1
        mov    dword ptr [ebp - 14h], eax
        movsx  eax, word ptr [ebp - 4]
        movsx  ecx, word ptr [ebp - 10h]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_1CD0
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*4 + 3ch], 0ffffh
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + eax*4 + 3eh], 0ffffh
        jmp    NEAR PTR loc_1_1D7A
loc_1_1CD0:
        movsx  eax, word ptr [ebp - 10h]
        cmp    eax, -1
        jne    NEAR PTR loc_1_1D0A
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 4], 0ffffh
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*4 + 3eh], ax
        jmp    NEAR PTR loc_1_1D7A
loc_1_1D0A:
        movsx  eax, word ptr [ebp - 4]
        cmp    eax, -1
        jne    NEAR PTR loc_1_1D44
        movsx  eax, word ptr [ebp - 10h]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 6], 0ffffh
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*4 + 3ch], ax
        jmp    NEAR PTR loc_1_1D7A
loc_1_1D44:
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 8], eax
        movsx  eax, word ptr [ebp - 10h]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 0ch], eax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
loc_1_1D7A:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 14h]
        lea    ecx, [ecx + ecx*2]
        db  02Bh, 004h, 08Dh, 008h, 000h, 000h, 000h
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        dec    word ptr [ecx + eax*2 + 50h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [eax + 4], 0ffffh
        mov    eax, dword ptr [ebp - 18h]
        mov    word ptr [eax + 6], 0ffffh
        mov    eax, dword ptr [ebp - 18h]
        test   dword ptr [eax], 0ffffff00h
        jne    NEAR PTR loc_1_1DE8
        push   17h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   5a9h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_1DE8:
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_1DF0
loc_1_1DF0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_1DF5:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 10h]
        dec    eax
        db  00Fh, 0AFh, 005h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_1E2F
loc_1_1E27:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 18h], eax
loc_1_1E2F:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_1FBF
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_1EAF
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffff00h
        cmp    eax, dword ptr [ebp - 8]
        jne    NEAR PTR loc_1_1E83
        mov    eax, dword ptr [ebp - 14h]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_1EAA
loc_1_1E83:
        push   23h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   60ah
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2020
loc_1_1EAA:
        jmp    NEAR PTR loc_1_1F7F
loc_1_1EAF:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 4], ecx
        ja     NEAR PTR loc_1_1EDD
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 18h], eax
        ja     NEAR PTR loc_1_1EDD
        mov    eax, dword ptr [ebp - 18h]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        test   edx, edx
        je     NEAR PTR loc_1_1F0A
loc_1_1EDD:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2020
loc_1_1F0A:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 4]
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffff00h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_1F52
        mov    eax, dword ptr [ebp - 14h]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_1F7F
loc_1_1F52:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2020
loc_1_1F7F:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_1FAA
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        shr    eax, 1
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        lea    ecx, [ecx + ecx*2]
        db  02Bh, 004h, 08Dh, 008h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], eax
loc_1_1FAA:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2025
        add    esp, 8
        jmp    NEAR PTR loc_1_1E27
loc_1_1FBF:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_2019
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0C9C
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 38h], 0
        je     NEAR PTR loc_1_1FF7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1397
        add    esp, 4
        jmp    NEAR PTR loc_1_2019
loc_1_1FF7:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2ch]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_2019
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_200E:
        add    eax, 3e8h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 38h], eax
loc_1_2019:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2020
loc_1_2020:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2025:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0ch]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_206E
loc_1_2068:
        inc    dword ptr [ebp - 18h]
        inc    dword ptr [ebp - 8]
loc_1_206E:
        cmp    dword ptr [ebp - 18h], 4
        ja     NEAR PTR loc_1_217D
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 004h, 000h, 000h, 000h
        movsx  ecx, word ptr [ebp + 0ch]
        xor    eax, ecx
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        test   byte ptr [eax], 1
        je     NEAR PTR loc_1_2126
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        shr    eax, 1
        cmp    eax, dword ptr [ebp - 8]
        jne    NEAR PTR loc_1_2126
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C54
        add    esp, 8
        movsx  eax, word ptr [ebp - 4]
        movsx  ecx, word ptr [ebp + 0ch]
        cmp    eax, ecx
        jge    NEAR PTR loc_1_20F9
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp + 0ch], ax
loc_1_20F9:
        mov    eax, dword ptr [ebp - 10h]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp - 10h]
        or     dword ptr [eax], 0eh
        mov    eax, dword ptr [ebp - 10h]
        and    dword ptr [eax], 0fffffffeh
        mov    eax, dword ptr [ebp - 10h]
        or     dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 14h]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp - 18h]
        add    eax, eax
        mov    ecx, dword ptr [ebp - 14h]
        or     dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_2178
loc_1_2126:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        mov    ecx, 0eh
        and    ecx, 0fffffffeh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_2173
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   17h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   6a7h
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_21D3
        db  0E9h, 005h, 000h, 000h, 000h
loc_1_2173:
        jmp    NEAR PTR loc_1_217D
loc_1_2178:
        jmp    NEAR PTR loc_1_2068
loc_1_217D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_2193
        mov    eax, ecx
loc_1_2193:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 20h]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_21AF
        mov    eax, ecx
loc_1_21AF:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A82
        add    esp, 8
loc_1_21D3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_21D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        db  039h, 005h, 030h, 000h, 000h, 000h
        jae    NEAR PTR loc_1_2227
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_249A
loc_1_2227:
        mov    dword ptr [ebp - 1ch], 0
        jmp    NEAR PTR loc_1_2236
loc_1_2233:
        inc    dword ptr [ebp - 1ch]
loc_1_2236:
        cmp    dword ptr [ebp - 1ch], 4
        ja     NEAR PTR loc_1_226E
        mov    eax, dword ptr [ebp - 1ch]
        lea    eax, [eax + eax*2]
        db  066h, 08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_2269
        jmp    NEAR PTR loc_1_226E
loc_1_2269:
        jmp    NEAR PTR loc_1_2233
loc_1_226E:
        mov    eax, dword ptr [ebp - 1ch]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        imul   eax, ecx
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 20h]
        cmp    dword ptr [eax + 10h], ecx
        jae    NEAR PTR loc_1_22CB
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_249A
loc_1_22CB:
        mov    word ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_22F0
loc_1_22E6:
        inc    word ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
loc_1_22F0:
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jge    NEAR PTR loc_1_2446
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2708
        add    esp, 14h
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_23E8
        mov    eax, dword ptr [ebp - 1ch]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 18h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 8], 0ch
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_238F
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_23AE
loc_1_238F:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_23A6
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_23AE
loc_1_23A6:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
loc_1_23AE:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_23D8
        mov    ax, word ptr [ebp + 18h]
        movsx  ecx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*8 + 5eh], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0ah], ax
        jmp    NEAR PTR loc_1_23E3
loc_1_23D8:
        mov    ax, word ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0ah], ax
loc_1_23E3:
        jmp    NEAR PTR loc_1_2441
loc_1_23E8:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_2441
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_2407
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], 0
loc_1_2407:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_241E
        mov    eax, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_2426
loc_1_241E:
        mov    eax, dword ptr [ebp + 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 28h], eax
loc_1_2426:
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        jmp    NEAR PTR loc_1_2446
loc_1_2441:
        jmp    NEAR PTR loc_1_22E6
loc_1_2446:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_2492
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp - 1ch]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        imul   eax, ecx
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0C9C
        add    esp, 8
        mov    dword ptr [ebp - 18h], eax
loc_1_2492:
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_249A
loc_1_249A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_249F:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 24h], 4
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        add    eax, 34h
        mov    word ptr [ebp + 0ch], ax
        mov    dword ptr [ebp - 18h], 0
        mov    dword ptr [ebp - 0ch], 0
loc_1_24DC:
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_24EB
loc_1_24E8:
        inc    dword ptr [ebp - 20h]
loc_1_24EB:
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [ebp - 20h], eax
        ja     NEAR PTR loc_1_253F
        mov    eax, dword ptr [ebp - 20h]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jae    NEAR PTR loc_1_2535
        mov    eax, dword ptr [ebp - 24h]
        cmp    dword ptr [ebp - 20h], eax
        jne    NEAR PTR loc_1_253A
loc_1_2535:
        jmp    NEAR PTR loc_1_253F
loc_1_253A:
        jmp    NEAR PTR loc_1_24E8
loc_1_253F:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2708
        add    esp, 14h
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_267E
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        and    ecx, 0ffffh
        add    ecx, 0ch
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 8], 0ch
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_25B4
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_25D3
loc_1_25B4:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_25CB
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_25D3
loc_1_25CB:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
loc_1_25D3:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_25FD
        mov    ax, word ptr [ebp + 14h]
        movsx  ecx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + ecx*8 + 5eh], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0ah], ax
        jmp    NEAR PTR loc_1_2608
loc_1_25FD:
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0ah], ax
loc_1_2608:
        mov    eax, dword ptr [ebp - 20h]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 008h, 000h, 000h, 000h
        add    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 0ch]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_2652
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_26DE
        db  0E9h, 01Bh, 000h, 000h, 000h
loc_1_2652:
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    word ptr [ebp + 0ch], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_26D9
loc_1_267E:
        cmp    dword ptr [ebp - 20h], 0
        jbe    NEAR PTR loc_1_2694
        mov    eax, dword ptr [ebp - 20h]
        dec    eax
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_26D9
loc_1_2694:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_26CF
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_26B3
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_1_26B9
loc_1_26B3:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 2ch], eax
loc_1_26B9:
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
loc_1_26CF:
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2703
loc_1_26D9:
        jmp    NEAR PTR loc_1_24DC
loc_1_26DE:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 38h], 0
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0C9C
        add    esp, 8
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        jmp    NEAR PTR loc_1_2703
loc_1_2703:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2708:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_272F
loc_1_272C:
        inc    dword ptr [ebp - 18h]
loc_1_272F:
        cmp    dword ptr [ebp - 18h], 4
        jbe    NEAR PTR loc_1_27AD
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [eax + 14h], ecx
        jne    NEAR PTR loc_1_2763
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2903
loc_1_2763:
        mov    eax, dword ptr [ebp + 0ch]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 008h, 000h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2908
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_2795
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_27AD
loc_1_2795:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_2903
loc_1_27AD:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_176C
        add    esp, 14h
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_27DF
        jmp    NEAR PTR loc_1_27E4
loc_1_27DF:
        jmp    NEAR PTR loc_1_272C
loc_1_27E4:
        jmp    NEAR PTR loc_1_27EC
loc_1_27E9:
        dec    dword ptr [ebp - 18h]
loc_1_27EC:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 18h], eax
        jbe    NEAR PTR loc_1_28A6
        mov    eax, dword ptr [ebp - 18h]
        dec    eax
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        db  08Bh, 004h, 085h, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 18h]
        movsx  ecx, word ptr [ecx]
        xor    eax, ecx
        mov    word ptr [ebp - 4], ax
        movsx  eax, word ptr [ebp - 4]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        and    eax, 0eh
        mov    ecx, 0eh
        and    ecx, 0fffffffeh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_284C
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 1
        jne    NEAR PTR loc_1_2881
loc_1_284C:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   17h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   8fdh
        call   sub_1_FAD7
        add    esp, 0ch
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_2903
loc_1_2881:
        mov    eax, dword ptr [ebp - 8]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        or     dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A82
        add    esp, 8
        jmp    NEAR PTR loc_1_27E9
loc_1_28A6:
        mov    eax, dword ptr [ebp - 14h]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, eax
        mov    ecx, dword ptr [ebp - 14h]
        or     dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_28CC
        mov    eax, ecx
loc_1_28CC:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 20h]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_28E8
        mov    eax, ecx
loc_1_28E8:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_2903
loc_1_2903:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_2908:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_2997
        call   sub_1_01D5
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 8]
        je     NEAR PTR loc_1_2997
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_2942
        mov    eax, ecx
loc_1_2942:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 0
        jne    NEAR PTR loc_1_2970
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_01F5
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ch], eax
loc_1_2970:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 18h], ecx
        jae    NEAR PTR loc_1_2990
        mov    eax, 3
        jmp    NEAR PTR loc_1_2CC7
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_2990:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2CC7
loc_1_2997:
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_29C0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 18h], ecx
        jae    NEAR PTR loc_1_29C0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, dword ptr [ecx + 18h]
        mov    dword ptr [ebp + 0ch], eax
loc_1_29C0:
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        lea    ecx, [eax - 1]
        add    ecx, dword ptr [ebp + 0ch]
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    ebx, eax
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_2A10
loc_1_29F6:
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        movsx  eax, ax
        movsx  ecx, word ptr [ebp - 10h]
        add    eax, ecx
        mov    word ptr [ebp - 10h], ax
loc_1_2A10:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 10h]
        movsx  ecx, word ptr [ebp - 10h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_2B74
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2B74
        movsx  eax, word ptr [ebp - 10h]
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        test   dword ptr [eax], 0ffffff00h
        jne    NEAR PTR loc_1_2B6F
        db  0A1h, 000h, 000h, 000h, 000h
        movsx  ecx, word ptr [ebp - 10h]
        imul   eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 4]
        shr    eax, 0ch
        mov    dword ptr [ebp - 0ch], eax
        push   0c0000000h
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_003A
        add    esp, 0ch
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_2A9C
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 14h], eax
        jmp    NEAR PTR loc_1_2AC0
loc_1_2A9C:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2B74
loc_1_2AC0:
        mov    word ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_2ACF
loc_1_2ACB:
        inc    word ptr [ebp - 14h]
loc_1_2ACF:
        mov    eax, 1000h
        sub    edx, edx
        db  0F7h, 035h, 000h, 000h, 000h, 000h
        movsx  eax, ax
        movsx  ecx, word ptr [ebp - 14h]
        cmp    eax, ecx
        jle    NEAR PTR loc_1_2B5C
        movsx  eax, word ptr [ebp - 14h]
        movsx  ecx, word ptr [ebp - 10h]
        add    eax, ecx
        shl    eax, 3
        add    eax, dword ptr [ebp + 8]
        add    eax, 5ah
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        and    dword ptr [eax], 0ffh
        movsx  eax, word ptr [ebp - 14h]
        db  00Fh, 0AFh, 005h, 000h, 000h, 000h, 000h
        add    eax, dword ptr [ebp - 8]
        and    eax, 0ffffff00h
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx], eax
        movsx  eax, word ptr [ebp - 14h]
        test   eax, eax
        jne    NEAR PTR loc_1_2B3F
        mov    eax, dword ptr [ebp - 18h]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp - 18h]
        or     dword ptr [eax], 8
        jmp    NEAR PTR loc_1_2B4B
loc_1_2B3F:
        mov    eax, dword ptr [ebp - 18h]
        and    dword ptr [eax], 0fffffff1h
        mov    eax, dword ptr [ebp - 18h]
        or     dword ptr [eax], 0eh
loc_1_2B4B:
        mov    eax, dword ptr [ebp - 18h]
        and    dword ptr [eax], 0fffffffeh
        mov    eax, dword ptr [ebp - 18h]
        or     dword ptr [eax], 1
        jmp    NEAR PTR loc_1_2ACB
loc_1_2B5C:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1A82
        add    esp, 8
        dec    dword ptr [ebp - 4]
loc_1_2B6F:
        jmp    NEAR PTR loc_1_29F6
loc_1_2B74:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_2B8A
        mov    eax, ecx
loc_1_2B8A:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 20h]
        cmp    eax, ecx
        ja     NEAR PTR loc_1_2BA6
        mov    eax, ecx
loc_1_2BA6:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 20h], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2CA0
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_2BE1
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_2BE1:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_2C26
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_2C26:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_2C6B
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 28h]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_2C6B:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ah, 4
        je     NEAR PTR loc_1_2C94
        mov    eax, dword ptr [ebp + 8]
        add    eax, 50h
        push   eax
        push   5
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_2C94:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2CC7
        db  0E9h, 027h, 000h, 000h, 000h
loc_1_2CA0:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 18h], ecx
        jae    NEAR PTR loc_1_2CC0
        mov    eax, 3
        jmp    NEAR PTR loc_1_2CC7
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_2CC0:
        xor    eax, eax
        jmp    NEAR PTR loc_1_2CC7
loc_1_2CC7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_2CE4
        jmp    NEAR PTR loc_1_2D54
loc_1_2CE4:
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_2D0D
loc_1_2CFF:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_2D0D:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_2D34
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [ebp + 10h]
        inc    word ptr [eax]
        jmp    NEAR PTR loc_1_2CFF
loc_1_2D34:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_2D54:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
sub_1_2D5C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 80h
        je     NEAR PTR loc_1_2D92
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_2D92:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_2DEC
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
loc_1_2DEC:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        cmp    eax, 31h
        je     NEAR PTR loc_1_2E50
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
loc_1_2E50:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 15h]
        and    ecx, 0ffh
        cmp    eax, ecx
        je     NEAR PTR loc_1_2EBF
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
loc_1_2EBF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_2F1F
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
loc_1_2F1F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_2F85
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
loc_1_2F85:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 10h
        push   eax
        push   2ch
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_24241
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_2FFD
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5bh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
loc_1_2FFD:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_30C8
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_2410C
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_3035
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_30C3
loc_1_3035:
        cmp    dword ptr [ebp - 0ch], -1
        jne    NEAR PTR loc_1_3090
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 50h
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0ffffffffh
        jmp    NEAR PTR loc_1_3110
        db  0E9h, 033h, 000h, 000h, 000h
loc_1_3090:
        cmp    dword ptr [ebp - 0ch], -3
        jne    NEAR PTR loc_1_30C3
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 5
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, 0fffffffdh
        jmp    NEAR PTR loc_1_3110
loc_1_30C3:
        jmp    NEAR PTR loc_1_30D1
loc_1_30C8:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
loc_1_30D1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ffh
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 17h], 2
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_3110
loc_1_3110:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3115:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 0ffh
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [eax + 17h], 2
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        cmp    eax, 10h
        jl     NEAR PTR loc_1_3196
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 1dh
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        mov    eax, 2
        jmp    NEAR PTR loc_1_36F1
loc_1_3196:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_31C9
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_320A
loc_1_31C9:
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5eh
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        mov    eax, 2
        jmp    NEAR PTR loc_1_36F1
loc_1_320A:
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3248
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ah], 1
loc_1_3248:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_01AC
        add    esp, 4
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_35C9
loc_1_3269:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 02Ah, 0D4h, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3281:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07FB
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3299:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_09AB
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_32B1:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_4D6A
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_32C9:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_50CE
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_32E1:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5297
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_32F9:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_53BE
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3311:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_542A
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3333:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5A43
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_334B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_5E7D
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3363:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_62A2
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_337B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_638F
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3393:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6558
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_33AB:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_67A6
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_33C3:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_69A3
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_33DB:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6CDF
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_33F3:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_6E1A
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_340B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_702A
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3423:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7259
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_343B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7474
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3453:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7631
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_346B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7958
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3483:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_7AE5
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_349B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_825A
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_34B3:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_8EFA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_34CB:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_99A8
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_34E3:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_9BB7
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_34FB:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_9FEA
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3513:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_A013
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_352B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_19B0C
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3543:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1B2B7
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_355B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1CA9C
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_3573:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1CCC0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_358B:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_23A17
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
loc_1_35A3:
        push   1
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_36AE
        db  0E9h, 0E5h, 000h, 000h, 000h
loc_1_35C9:
        cmp    dword ptr [ebp - 10h], 35h
        ja     NEAR PTR loc_1_35A3
        mov    eax, dword ptr [ebp - 10h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 023h, 023h, 001h, 002h, 003h, 004h, 005h, 006h, 023h, 007h, 007h, 023h, 008h, 008h, 008h
        db  008h, 008h, 023h, 023h, 009h, 00Ah, 00Bh, 00Ch, 023h, 00Dh, 00Eh, 00Fh, 010h, 011h, 012h, 023h
        db  013h, 014h, 015h, 016h, 023h, 023h, 017h, 018h, 019h, 01Ah, 023h, 023h, 01Bh, 01Ch, 023h, 023h
        db  01Dh, 01Eh, 01Fh, 020h, 021h, 022h
loc_1_36AE:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_36CC
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0ah], 0
loc_1_36CC:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_F2C4
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_36F1
loc_1_36F1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_36F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 2
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_373D
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
loc_1_373D:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_376C
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_376C:
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_2410C
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], -1
        jne    NEAR PTR loc_1_379F
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 1
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
        db  0E9h, 01Bh, 000h, 000h, 000h
loc_1_379F:
        cmp    dword ptr [ebp - 0ch], -3
        jne    NEAR PTR loc_1_37BA
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 5
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
loc_1_37BA:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_380F
        push   4
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_1_1072E
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_37F0
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_380F
loc_1_37F0:
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 6
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
loc_1_380F:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 15h]
        mov    byte ptr [ebp - 10h], al
        mov    eax, 1
        mov    cl, byte ptr [ebp - 10h]
        shl    eax, cl
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_3833
loc_1_382D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
loc_1_3833:
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_1_3A68
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 80h
        je     NEAR PTR loc_1_3880
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 16h]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_3880:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        cmp    eax, ecx
        je     NEAR PTR loc_1_38EF
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 5fh
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
loc_1_38EF:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_39B9
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_2410C
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], -1
        jne    NEAR PTR loc_1_3976
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 8
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 50h
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 3
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
        db  0E9h, 03Eh, 000h, 000h, 000h
loc_1_3976:
        cmp    dword ptr [ebp - 0ch], -3
        jne    NEAR PTR loc_1_39B4
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    byte ptr [eax + 2], 7
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 5
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
loc_1_39B4:
        jmp    NEAR PTR loc_1_39C0
loc_1_39B9:
        mov    dword ptr [ebp - 8], 0
loc_1_39C0:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3A1F
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_2D5C
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_1_3A1F
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_242B3
        add    esp, 8
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_3A18
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_242B3
        add    esp, 8
loc_1_3A18:
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
loc_1_3A1F:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_3115
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 0ch], eax
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_1_3A63
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 0ch], 4
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_3A68
loc_1_3A63:
        jmp    NEAR PTR loc_1_382D
loc_1_3A68:
        xor    eax, eax
        jmp    NEAR PTR loc_1_3A6F
loc_1_3A6F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3A74:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 2
        mov    dword ptr [ebp - 10h], eax
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_3AB7
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
loc_1_3AB7:
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_3AC6
loc_1_3AC2:
        inc    word ptr [ebp - 8]
loc_1_3AC6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_3B8B
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_3B86
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffffh
        cmp    eax, 4
        jne    NEAR PTR loc_1_3B86
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01A6
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_3B2F
loc_1_3B27:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
loc_1_3B2F:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_3B77
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_3B72
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 0ch], ecx
        jne    NEAR PTR loc_1_3B72
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_46AA
        add    esp, 8
        jmp    NEAR PTR loc_1_3B77
loc_1_3B72:
        jmp    NEAR PTR loc_1_3B27
loc_1_3B77:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_1_01AC
        add    esp, 4
loc_1_3B86:
        jmp    NEAR PTR loc_1_3AC2
loc_1_3B8B:
        xor    eax, eax
        jmp    NEAR PTR loc_1_3B92
loc_1_3B92:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h
sub_1_3B98:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   0bb8h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 30h
        push   eax
        call   sub_1_EACD
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_3BD7
        push   2eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   3bh
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_3BD7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_3C16
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        inc    word ptr [eax]
loc_1_3C16:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 38h
        push   eax
        call   sub_1_19726
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_3C4F
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_416F
        add    esp, 10h
        jmp    NEAR PTR loc_1_3C16
loc_1_3C4F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3C65
loc_1_3C5D:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_3C65:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_3DB1
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 2
        je     NEAR PTR loc_1_3C97
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 3
        jne    NEAR PTR loc_1_3D1D
loc_1_3C97:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 6]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_3D1D
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
        push   1
        push   1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_3D0C
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_3D18
loc_1_3D0C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_3D18:
        jmp    NEAR PTR loc_1_3DAC
loc_1_3D1D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        cmp    eax, 6
        jne    NEAR PTR loc_1_3DAC
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 6]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_3DAC
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
        push   1
        push   1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_3DA0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_3DAC
loc_1_3DA0:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_3DAC:
        jmp    NEAR PTR loc_1_3C5D
loc_1_3DB1:
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 34h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4560
        add    esp, 14h
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4560
        add    esp, 14h
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 48h
        push   eax
        call   sub_1_4508
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_3E0B:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   0bb8h
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 18h
        push   eax
        call   sub_1_EACD
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_3E4D
        push   2eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0b9h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_3E4D:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_3E8C
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 18h]
        inc    word ptr [eax]
loc_1_3E8C:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_3ECB
        mov    al, byte ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 18h]
        inc    word ptr [eax]
loc_1_3ECB:
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_460F
        add    esp, 14h
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4560
        add    esp, 14h
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 30h
        push   eax
        call   sub_1_4508
        add    esp, 0ch
        mov    al, byte ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 28h
        push   eax
        call   sub_1_4508
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3F53
loc_1_3F4B:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_3F53:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_40A6
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    byte ptr [ebp - 14h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 2
        je     NEAR PTR loc_1_3F85
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 3
        jne    NEAR PTR loc_1_402E
loc_1_3F85:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 0ch], ecx
        jne    NEAR PTR loc_1_402E
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_3FF4
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        push   1
        push   1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
loc_1_3FF4:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_401D
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_4029
loc_1_401D:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_4029:
        jmp    NEAR PTR loc_1_40A1
loc_1_402E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 7
        je     NEAR PTR loc_1_404A
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 8
        jne    NEAR PTR loc_1_40A1
loc_1_404A:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [eax + 10h], ecx
        jne    NEAR PTR loc_1_40A1
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_4095
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_40A1
loc_1_4095:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_40A1:
        jmp    NEAR PTR loc_1_3F4B
loc_1_40A6:
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 38h
        push   eax
        call   sub_1_19800
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_40E1
        push   1
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        jmp    NEAR PTR loc_1_40A6
loc_1_40E1:
        cmp    dword ptr [ebp + 1ch], 0
        je     NEAR PTR loc_1_40F1
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 6ch]
loc_1_40F1:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_4129
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_4129
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 6bh]
        mov    eax, dword ptr [ebp + 0ch]
        dec    byte ptr [eax + 65h]
        jmp    NEAR PTR loc_1_415A
loc_1_4129:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_1_415A
        push   12h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   127h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_415A:
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_119D8
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_416F:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   0bb8h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 18h
        push   eax
        call   sub_1_EACD
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_41B1
        push   2eh
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   156h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_41B1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_41F0
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        inc    word ptr [eax]
loc_1_41F0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_422F
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 10h]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        inc    word ptr [eax]
loc_1_422F:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 54h
        push   eax
        call   sub_1_19726
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_426E
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3E0B
        add    esp, 18h
        jmp    NEAR PTR loc_1_422F
loc_1_426E:
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_460F
        add    esp, 14h
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4560
        add    esp, 14h
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 30h
        push   eax
        call   sub_1_4508
        add    esp, 0ch
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 28h
        push   eax
        call   sub_1_4508
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_42F6
loc_1_42EE:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
loc_1_42F6:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_4462
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 1fh
        mov    byte ptr [ebp - 14h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 2
        je     NEAR PTR loc_1_4328
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 3
        jne    NEAR PTR loc_1_43F8
loc_1_4328:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0ch], ecx
        jne    NEAR PTR loc_1_43F8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_439C
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        push   1
        push   1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        jmp    NEAR PTR loc_1_43BE
loc_1_439C:
        push   27h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   19bh
        call   sub_1_FAD7
        add    esp, 0ch
        jmp    NEAR PTR loc_1_4503
loc_1_43BE:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_43E7
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_43F3
loc_1_43E7:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_43F3:
        jmp    NEAR PTR loc_1_445D
loc_1_43F8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        cmp    eax, 9
        jne    NEAR PTR loc_1_445D
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], ecx
        jne    NEAR PTR loc_1_445D
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_4451
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_192B6
        add    esp, 8
        jmp    NEAR PTR loc_1_445D
loc_1_4451:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1923E
        add    esp, 4
loc_1_445D:
        jmp    NEAR PTR loc_1_42EE
loc_1_4462:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 38h
        push   eax
        call   sub_1_19800
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_449D
        push   1
        push   1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        call   sub_1_1DF5
        add    esp, 10h
        jmp    NEAR PTR loc_1_4462
loc_1_449D:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_10A89
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_1_44D4
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_44F1
loc_1_44D4:
        push   13h
        call   sub_1_02ED
        add    esp, 4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   1c3h
        call   sub_1_FAD7
        add    esp, 0ch
loc_1_44F1:
        mov    eax, dword ptr [ebp + 8]
        dec    byte ptr [eax + 69h]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_11D37
        add    esp, 4
loc_1_4503:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4508:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
loc_1_4511:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_455B
        mov    al, byte ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        inc    word ptr [eax]
        jmp    NEAR PTR loc_1_4511
loc_1_455B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4560:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_4583
loc_1_457D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_4583:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_460A
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [ecx + 0bh]
        and    ecx, 0ffh
        cmp    eax, ecx
        jne    NEAR PTR loc_1_45F7
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        mov    al, byte ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        inc    word ptr [eax]
        jmp    NEAR PTR loc_1_4605
loc_1_45F7:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_4605:
        jmp    NEAR PTR loc_1_457D
loc_1_460A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_460F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
loc_1_4618:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_19A9D
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_46A5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_4655
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 40h], ecx
        jne    NEAR PTR loc_1_4668
loc_1_4655:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 3ch
        push   eax
        call   sub_1_1962D
        add    esp, 8
loc_1_4668:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_22F01
        add    esp, 4
        mov    al, byte ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 17h], 80h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    eax, 2
        push   eax
        call   sub_1_19871
        add    esp, 8
        mov    eax, dword ptr [ebp + 14h]
        inc    word ptr [eax]
        jmp    NEAR PTR loc_1_4618
loc_1_46A5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_46AA:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_46E0
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0bh]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_46E0:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 50h], 0
        je     NEAR PTR loc_1_47AD
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8bh]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_4719
loc_1_4711:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_4719:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_4731
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 50h]
        or     dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_4711
loc_1_4731:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_477A
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E359
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_4775
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4775:
        jmp    NEAR PTR loc_1_47AD
loc_1_477A:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_47AD
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_47AD:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_4843
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 54h], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_4810
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E440
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_480B
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_480B:
        jmp    NEAR PTR loc_1_4843
loc_1_4810:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_4843
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4843:
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        lea    eax, [ebp - 0ch]
        push   eax
        push   0bh
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B98
        add    esp, 10h
        cmp    dword ptr [ebp - 0ah], 0
        je     NEAR PTR loc_1_487B
        mov    eax, dword ptr [ebp - 0ah]
        push   eax
        call   sub_1_24419
        add    esp, 4
loc_1_487B:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_115A5
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_48B5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
loc_1_48B5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_48BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28EC2
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_493D
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 10h
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
        jmp    NEAR PTR loc_1_4A6D
loc_1_493D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_494C
loc_1_4948:
        add    dword ptr [ebp - 14h], 2eh
loc_1_494C:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_1_4A5A
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 6
        push   eax
        call   sub_1_E002
        add    esp, 8
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 1eh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_28F82
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_49E9
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        cmp    eax, 4
        ja     NEAR PTR loc_1_49E9
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 1eh], ecx
        jne    NEAR PTR loc_1_49E9
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 1eh], 0
        je     NEAR PTR loc_1_49D1
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 1eh], 1
        jne    NEAR PTR loc_1_49E9
loc_1_49D1:
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_4A72
        add    esp, 8
        jmp    NEAR PTR loc_1_4A55
loc_1_49E9:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_4A06
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
loc_1_4A06:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 20h
        je     NEAR PTR loc_1_4A42
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 6
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        dec    eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_4A42:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 22h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_4A72
        add    esp, 8
loc_1_4A55:
        jmp    NEAR PTR loc_1_4948
loc_1_4A5A:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_28F88
        mov    eax, 1
        jmp    NEAR PTR loc_1_4A6D
loc_1_4A6D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4A72:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_4A8A
loc_1_4A86:
        inc    word ptr [ebp - 4]
loc_1_4A8A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jle    NEAR PTR loc_1_4AC9
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    al, byte ptr [ebp + eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx]
        mov    byte ptr [ecx + edx], al
        jmp    NEAR PTR loc_1_4A86
loc_1_4AC9:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_4BBE
loc_1_4ADD:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 8
        je     NEAR PTR loc_1_4B0E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_4B0E:
        jmp    NEAR PTR loc_1_4BE1
loc_1_4B13:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 8
        je     NEAR PTR loc_1_4B45
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_4B45:
        jmp    NEAR PTR loc_1_4BE1
loc_1_4B4A:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 8
        je     NEAR PTR loc_1_4B76
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 0ch
loc_1_4B76:
        jmp    NEAR PTR loc_1_4BE1
loc_1_4B7B:
        xor    eax, eax
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   al, 8
        je     NEAR PTR loc_1_4BB4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        push   10h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 6
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 14h
loc_1_4BB4:
        jmp    NEAR PTR loc_1_4BE1
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_4BBE:
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_4ADD
        cmp    dword ptr [ebp - 8], 2
        je     NEAR PTR loc_1_4B13
        cmp    dword ptr [ebp - 8], 4
        je     NEAR PTR loc_1_4B4A
        jmp    NEAR PTR loc_1_4B7B
loc_1_4BE1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4BE6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx], ax
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 8], ax
        db  0A0h, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 68h], al
        db  0A0h, 005h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 6ah], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 6ah]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 6ch], al
        db  0A0h, 006h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 6dh], al
        db  0A0h, 007h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 6eh], al
        db  0A0h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 6fh], al
        db  0A0h, 009h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 70h], al
        db  0A0h, 00Ah, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 71h], al
        db  0A0h, 00Bh, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 72h], al
        db  0A0h, 00Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 73h], al
        db  0A0h, 00Dh, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 74h], al
        db  0A0h, 00Eh, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 75h], al
        db  0A0h, 00Fh, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 76h], al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_4C9C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 34h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_4CC0
loc_1_4CBA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
loc_1_4CC0:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_4D65
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    cx, word ptr [ecx + 0eh]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 0ch]
        and    edx, 0ffffh
        cmp    ecx, edx
        jl     NEAR PTR loc_1_4CFC
        mov    ecx, edx
loc_1_4CFC:
        sub    eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 4]
        mov    word ptr [ecx + 0eh], ax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_4D52
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 34h
        push   eax
        call   sub_1_198DA
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], eax
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        jmp    NEAR PTR loc_1_4D60
loc_1_4D52:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_4D60:
        jmp    NEAR PTR loc_1_4CBA
loc_1_4D65:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_4D6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F4B8
        add    esp, 8
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        je     NEAR PTR loc_1_4DB3
        mov    al, byte ptr [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_4FD0
loc_1_4DB3:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 14h]
        and    eax, 0ffh
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*2]
        shl    eax, 3
        db  003h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 50h], 0
        je     NEAR PTR loc_1_4EDE
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 50h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8bh]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_4E0E
loc_1_4E06:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
loc_1_4E0E:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_4E26
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 50h]
        or     dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_4E06
loc_1_4E26:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_4E8D
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E359
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_4E88
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_4FD0
loc_1_4E88:
        jmp    NEAR PTR loc_1_4EDE
loc_1_4E8D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_4EDE
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_4FD0
loc_1_4EDE:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 54h], 0
        je     NEAR PTR loc_1_4FB0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 54h], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5ch], 1
        jne    NEAR PTR loc_1_4F5F
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 54h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E440
        add    esp, 8
        test   eax, eax
        je     NEAR PTR loc_1_4F5A
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_4FD0
loc_1_4F5A:
        jmp    NEAR PTR loc_1_4FB0
loc_1_4F5F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_E527
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_4FB0
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_1F9F2
        add    esp, 4
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        push   7
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        and    eax, 0ffffh
        jmp    NEAR PTR loc_1_4FD0
loc_1_4FB0:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_14A84
        add    esp, 10h
        xor    eax, eax
        jmp    NEAR PTR loc_1_4FD0
loc_1_4FD0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_4FD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 17h]
        and    eax, 0ffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_5038
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [eax + 8], 0
        je     NEAR PTR loc_1_5038
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 4]
        mov    al, byte ptr [eax + 15h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_5038
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        call   sub_1_1A825
        add    esp, 10h
loc_1_5038:
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_117CE
        add    esp, 4
        lea    eax, [ebp - 0ch]
        push   eax
        push   0bh
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B98
        add    esp, 10h
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_5077
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 40h
        push   eax
        call   sub_1_19871
        add    esp, 8
loc_1_5077:
        mov    al, byte ptr [ebp + 14h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_F797
        add    esp, 10h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 14h
        push   eax
        call   sub_1_1962D
        add    esp, 8
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_115A5
        add    esp, 8
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_50C9
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_18FEF
        add    esp, 4
loc_1_50C9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
_LTEXT ends
        end
