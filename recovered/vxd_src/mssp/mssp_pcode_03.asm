; mssp_pcode_03.asm -- MSSP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

SECPROV_Service_00:
        mov    eax, 400h
        clc
        ret
SECPROV_W32_DEVICEIOCONTROL:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 0
        jne    SHORT loc_3_0016
        mov    ecx, 400h
        jmp    SHORT loc_3_001B
loc_3_0016:
        cmp    ecx, -1
        jne    SHORT loc_3_001F
loc_3_001B:
        xor    eax, eax
        clc
        ret
loc_3_001F:
        cmp    ecx, 64h
        jl     SHORT loc_3_003E
        cmp    ecx, 6ch
        jge    SHORT loc_3_003E
        sub    ecx, 64h
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        db  0FFh, 014h, 08Dh, 000h, 000h, 000h, 000h
        add    esp, 8
        clc
        ret
loc_3_003E:
        mov    eax, 1
        clc
        ret
sub_3_0045:
        movzx  eax, byte ptr [esp + 4]
        db  0CDh, 020h, 0D1h, 000h, 001h, 000h
        sbb    eax, eax
        inc    eax
        ret    4
sub_3_0056:
        db  0CDh, 020h, 00Ch, 080h, 081h, 004h
sub_3_005C:
        db  0CDh, 020h, 00Ah, 080h, 081h, 004h
sub_3_0062:
        db  0CDh, 020h, 00Bh, 080h, 081h, 004h
sub_3_0068:
        db  0CDh, 020h, 009h, 080h, 081h, 004h
sub_3_006E:
        db  0CDh, 020h, 00Fh, 080h, 081h, 004h
sub_3_0074:
        db  0CDh, 020h, 001h, 080h, 080h, 004h
sub_3_007A:
        db  0CDh, 020h, 019h, 080h, 040h, 000h
        db  0CDh, 020h, 01Ah, 080h, 040h, 000h
sub_3_0086:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
sub_3_008C:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
sub_3_0092:
        db  0CDh, 020h, 043h, 080h, 040h, 000h
sub_3_0098:
        db  0CDh, 020h, 00Bh, 080h, 091h, 004h
sub_3_009E:
        db  0CDh, 020h, 00Fh, 080h, 091h, 004h
sub_3_00A4:
        db  0CDh, 020h, 00Dh, 080h, 091h, 004h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00B0:
        push   ebx
        push   esi
        push   edi
        call   sub_3_0098
        or     eax, eax
        jne    SHORT loc_3_00DB
        mov    edi, dword ptr [esp + 10h]
        push   edi
        call   sub_3_18A8
        add    esp, 4
        lea    ebx, [edi + eax - 1]
        cmp    byte ptr [ebx], 20h
        jne    SHORT loc_3_00D8
loc_3_00D2:
        dec    ebx
        cmp    byte ptr [ebx], 20h
        je     SHORT loc_3_00D2
loc_3_00D8:
        inc    ebx
        jmp    SHORT loc_3_0113
loc_3_00DB:
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        mov    esi, edi
        cmp    byte ptr [edi], bl
        je     SHORT loc_3_010D
loc_3_00E7:
        mov    al, byte ptr [esi]
        push   eax
        call   sub_3_0045
        or     eax, eax
        je     SHORT loc_3_00F8
        sub    ebx, ebx
        inc    esi
        jmp    SHORT loc_3_0107
loc_3_00F8:
        cmp    byte ptr [esi], 20h
        je     SHORT loc_3_0101
        sub    ebx, ebx
        jmp    SHORT loc_3_0107
loc_3_0101:
        or     ebx, ebx
        jne    SHORT loc_3_0107
        mov    ebx, esi
loc_3_0107:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_00E7
loc_3_010D:
        or     ebx, ebx
        jne    SHORT loc_3_0113
        mov    ebx, esi
loc_3_0113:
        mov    esi, dword ptr [esp + 14h]
        cmp    esi, edi
        je     SHORT loc_3_012A
        sub    ebx, edi
        push   ebx
        push   edi
        push   esi
        call   sub_3_18B0
        add    esp, 0ch
        add    ebx, esi
loc_3_012A:
        mov    byte ptr [ebx], 0
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_3_0135:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_3_0153
        cmp    byte ptr [esi], 0
        je     SHORT loc_3_0153
        push   esi
        call   sub_3_18A8
        add    esp, 4
        mov    ebp, eax
        jmp    SHORT loc_3_015F
loc_3_0153:
        db  0BEh, 054h, 002h, 000h, 000h
        db  08Bh, 02Dh, 000h, 009h, 000h, 000h
        dec    ebp
loc_3_015F:
        mov    edi, dword ptr [esp + 18h]
        or     edi, edi
        jne    SHORT loc_3_0169
        jmp    SHORT loc_3_01AC
loc_3_0169:
        push   edi
        call   sub_3_18A8
        add    esp, 4
        mov    ebx, eax
        or     ebp, ebp
        je     SHORT loc_3_01AC
        cmp    ebp, 0fh
        ja     SHORT loc_3_01AC
        or     ebx, ebx
        je     SHORT loc_3_01AC
        cmp    ebx, 14h
        ja     SHORT loc_3_01AC
        push   ebp
        push   esi
        inc    ebx
        mov    esi, dword ptr [esp + 24h]
        push   esi
        call   sub_3_18B0
        add    esp, 0ch
        lea    eax, [esi + ebp]
        push   ebx
        mov    byte ptr [eax], 5ch
        push   edi
        inc    eax
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        sub    eax, eax
        jmp    SHORT loc_3_01B1
loc_3_01AC:
        mov    eax, 57h
loc_3_01B1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_3_01B8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        db  08Bh, 02Dh, 058h, 000h, 000h, 000h
        dec    ebp
        js     SHORT loc_3_01FB
        mov    esi, dword ptr [esp + 14h]
loc_3_01CB:
        lea    eax, [edi + ebp]
        cdq
        sub    eax, edx
        sar    eax, 1
        mov    ebx, eax
        push   1
        imul   eax, eax, 0ch
        db  08Bh, 00Dh, 050h, 000h, 000h, 000h
        push   dword ptr [eax + ecx]
        push   esi
        call   sub_3_00A4
        or     eax, eax
        jge    SHORT loc_3_01F2
        lea    ebp, [ebx - 1]
        jmp    SHORT loc_3_01F7
loc_3_01F2:
        jle    SHORT loc_3_020A
        lea    edi, [ebx + 1]
loc_3_01F7:
        cmp    ebp, edi
        jge    SHORT loc_3_01CB
loc_3_01FB:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], edi
        sub    eax, eax
loc_3_0203:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_3_020A:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], ebx
        mov    eax, 1
        jmp    SHORT loc_3_0203
sub_3_0217:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 34h]
        call   sub_3_01B8
        or     eax, eax
        je     SHORT loc_3_0246
        mov    eax, dword ptr [esp + 18h]
        imul   eax, eax, 0ch
        db  08Bh, 00Dh, 050h, 000h, 000h, 000h
        inc    dword ptr [eax + ecx + 4]
        jmp    NEAR PTR loc_3_0451
loc_3_0246:
        push   dword ptr [esp + 2ch]
        call   sub_3_18A8
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 14h], eax
        movzx  eax, word ptr [esp + 34h]
        mov    dword ptr [esp + 10h], eax
        mov    ecx, eax
        lea    edi, [ecx*4 + 4]
        push   200h
        push   edi
        call   sub_3_183C
        mov    dword ptr [esp + 2ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_0289
        mov    eax, 8
        jmp    NEAR PTR loc_3_0453
loc_3_0289:
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        sub    ebx, ebx
        cmp    word ptr [esp + 34h], bx
        je     SHORT loc_3_02D8
        mov    esi, dword ptr [esp + 38h]
        mov    ebp, eax
        add    ebp, 4
loc_3_02A5:
        or     ebx, ebx
        je     SHORT loc_3_02B1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp], eax
        jmp    SHORT loc_3_02B8
loc_3_02B1:
        mov    dword ptr [ebp], 0
loc_3_02B8:
        push   esi
        call   sub_3_18A8
        mov    ecx, dword ptr [esp + 18h]
        add    esp, 4
        add    ecx, eax
        inc    ecx
        add    dword ptr [ebp], ecx
        add    esi, 15h
        add    ebp, 4
        inc    ebx
        cmp    ebx, dword ptr [esp + 10h]
        jl     SHORT loc_3_02A5
loc_3_02D8:
        cmp    word ptr [esp + 34h], 0
        je     SHORT loc_3_02EB
        mov    eax, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 10h]
        add    edi, dword ptr [eax + ecx*4]
loc_3_02EB:
        push   dword ptr [esp + 30h]
        call   sub_3_18A8
        add    esp, 4
        lea    edi, [eax + edi + 1]
        push   0
        push   edi
        push   dword ptr [esp + 2ch]
        call   sub_3_184C
        mov    dword ptr [esp + 28h], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_032A
        push   0
        push   dword ptr [esp + 28h]
        call   sub_3_1844
        add    esp, 8
        mov    eax, 8
        jmp    NEAR PTR loc_3_0453
loc_3_032A:
        db  0A1h, 054h, 000h, 000h, 000h
        db  039h, 005h, 058h, 000h, 000h, 000h
        jne    SHORT loc_3_037F
        db  083h, 005h, 054h, 000h, 000h, 000h, 00Ah
        push   0
        db  0A1h, 054h, 000h, 000h, 000h
        imul   eax, eax, 0ch
        push   eax
        db  0FFh, 035h, 050h, 000h, 000h, 000h
        call   sub_3_184C
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_037A
        push   0
        push   dword ptr [esp + 20h]
        call   sub_3_1844
        add    esp, 8
        db  083h, 02Dh, 054h, 000h, 000h, 000h, 00Ah
        mov    eax, 8
        jmp    NEAR PTR loc_3_0453
loc_3_037A:
        db  0A3h, 050h, 000h, 000h, 000h
loc_3_037F:
        mov    edx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 10h]
        lea    ecx, [edx + eax*4 + 4]
        mov    dword ptr [esp + 20h], ecx
        sub    esi, esi
        cmp    word ptr [esp + 34h], si
        je     SHORT loc_3_03D7
        mov    ebx, dword ptr [esp + 38h]
        mov    edi, edx
loc_3_039E:
        or     esi, esi
        mov    eax, 0
        je     SHORT loc_3_03A9
        mov    eax, dword ptr [edi]
loc_3_03A9:
        mov    ebp, dword ptr [esp + 20h]
        add    ebp, eax
        push   ebp
        push   dword ptr [esp + 30h]
        call   sub_3_009E
        mov    eax, dword ptr [esp + 14h]
        add    eax, ebp
        push   eax
        mov    byte ptr [eax - 1], 5ch
        push   ebx
        call   sub_3_009E
        add    ebx, 15h
        add    edi, 4
        inc    esi
        cmp    esi, dword ptr [esp + 10h]
        jl     SHORT loc_3_039E
loc_3_03D7:
        cmp    word ptr [esp + 34h], 0
        mov    eax, 0
        je     SHORT loc_3_03EF
        mov    edx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edx + ecx*4]
loc_3_03EF:
        mov    ebx, dword ptr [esp + 20h]
        push   -1
        add    ebx, eax
        push   dword ptr [esp + 34h]
        push   ebx
        call   sub_3_18A0
        add    esp, 0ch
        db  08Bh, 00Dh, 058h, 000h, 000h, 000h
        dec    ecx
        cmp    ecx, dword ptr [esp + 18h]
        jl     SHORT loc_3_042E
        mov    edx, ecx
        imul   edx, edx, 0ch
loc_3_0416:
        db  08Bh, 035h, 050h, 000h, 000h, 000h
        add    esi, edx
        sub    edx, 0ch
        lea    edi, [esi + 0ch]
        movsd
        movsd
        movsd
        dec    ecx
        cmp    ecx, dword ptr [esp + 18h]
        jge    SHORT loc_3_0416
loc_3_042E:
        mov    eax, dword ptr [esp + 18h]
        imul   eax, eax, 0ch
        db  003h, 005h, 050h, 000h, 000h, 000h
        db  0FFh, 005h, 058h, 000h, 000h, 000h
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], 1
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 8], edx
loc_3_0451:
        sub    eax, eax
loc_3_0453:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret    10h
sub_3_045D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   0
        push   dword ptr [esi + 8]
        call   sub_3_1844
        add    esp, 8
        db  02Bh, 035h, 050h, 000h, 000h, 000h
        mov    ecx, 0ch
        mov    eax, esi
        cdq
        idiv   ecx
        db  08Bh, 00Dh, 058h, 000h, 000h, 000h
        dec    ecx
        cmp    ecx, eax
        jle    SHORT loc_3_04AD
        mov    edx, eax
        imul   edx, edx, 0ch
loc_3_0490:
        db  08Bh, 03Dh, 050h, 000h, 000h, 000h
        add    edi, edx
        add    edx, 0ch
        lea    esi, [edi + 0ch]
        movsd
        movsd
        movsd
        inc    eax
        db  08Bh, 00Dh, 058h, 000h, 000h, 000h
        dec    ecx
        cmp    ecx, eax
        jg     SHORT loc_3_0490
loc_3_04AD:
        db  0FFh, 00Dh, 058h, 000h, 000h, 000h
        db  0A1h, 054h, 000h, 000h, 000h
        sub    eax, 14h
        db  03Bh, 005h, 058h, 000h, 000h, 000h
        jl     SHORT loc_3_04F5
        db  083h, 02Dh, 054h, 000h, 000h, 000h, 00Ah
        push   0
        db  0A1h, 054h, 000h, 000h, 000h
        imul   eax, eax, 0ch
        push   eax
        db  0FFh, 035h, 050h, 000h, 000h, 000h
        call   sub_3_184C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_04EE
        db  0A3h, 050h, 000h, 000h, 000h
        jmp    SHORT loc_3_04F5
loc_3_04EE:
        db  083h, 005h, 054h, 000h, 000h, 000h, 00Ah
loc_3_04F5:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    4
sub_3_04FC:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    edi, edi
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        push   3
        push   ebx
        push   dword ptr [esp + 48h]
        call   sub_3_1254
        mov    esi, eax
        cmp    esi, ebx
        je     SHORT loc_3_0544
        cmp    esi, 4d5h
        je     SHORT loc_3_0544
        cmp    esi, 0b7h
        jne    NEAR PTR loc_3_0778
        jmp    SHORT loc_3_0549
loc_3_0544:
        mov    ebx, 1
loc_3_0549:
        push   0
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0AAh, 0FBh, 0FCh, 0FFh
        or     eax, eax
        jne    SHORT loc_3_055F
        jmp    NEAR PTR loc_3_0773
loc_3_055F:
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [esp + 44h]
        add    eax, 10h
        push   eax
        call   dword ptr [esp + 48h]
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_3_0778
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 8]
        add    eax, 10h
        push   eax
        lea    eax, [esp + 30h]
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        add    eax, 10h
        push   eax
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 05Ch, 0FBh, 0FCh, 0FFh
        cmp    eax, 2
        jne    SHORT loc_3_05CF
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 0ch]
        add    eax, 20h
        push   eax
        mov    edi, 1
        lea    eax, [esp + 20h]
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    ebp, dword ptr [eax + 0ch]
        jmp    SHORT loc_3_05D3
loc_3_05CF:
        mov    ebp, dword ptr [esp + 14h]
loc_3_05D3:
        or     ebx, ebx
        jne    SHORT loc_3_0601
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   0
        mov    ebx, 2
        push   dword ptr [esp + 48h]
        call   sub_3_1254
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_060A
        cmp    esi, 4d5h
        je     SHORT loc_3_060A
        jmp    NEAR PTR loc_3_0778
loc_3_0601:
        or     edi, edi
        jne    SHORT loc_3_060E
        jmp    NEAR PTR loc_3_0773
loc_3_060A:
        or     edi, edi
        je     SHORT loc_3_061D
loc_3_060E:
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 44h]
        push   eax
        call   dword ptr [esp + 48h]
        mov    esi, eax
loc_3_061D:
        cmp    ebx, 2
        jne    SHORT loc_3_063E
        lea    eax, [esp + 18h]
        or     edi, edi
        jne    SHORT loc_3_062E
        lea    eax, [esp + 28h]
loc_3_062E:
        push   eax
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0C6h, 0FAh, 0FCh, 0FFh
        mov    ebx, eax
        jmp    SHORT loc_3_0640
loc_3_063E:
        sub    ebx, ebx
loc_3_0640:
        or     edi, edi
        je     NEAR PTR loc_3_06D7
        or     esi, esi
        jne    NEAR PTR loc_3_06D7
        db  0A1h, 050h, 002h, 000h, 000h
        cmp    dword ptr [esp + 10h], eax
        jne    SHORT loc_3_0666
        lea    eax, [esp + 18h]
        push   ebp
        push   eax
        call   sub_3_1720
loc_3_0666:
        cmp    ebx, 1
        jne    SHORT loc_3_06A6
        push   0
        push   1
        push   dword ptr [esp + 18h]
        db  0E8h, 088h, 0FAh, 0FCh, 0FFh
        cmp    ebx, 1
        jne    SHORT loc_3_06A6
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 8]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [esp + 18h]
        add    eax, 20h
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
loc_3_06A6:
        mov    eax, dword ptr [esp + 10h]
        push   ebp
        mov    dword ptr [eax + 8], ebp
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 18h]
        add    eax, 10h
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        cmp    ebx, 1
        sbb    eax, eax
        add    eax, 2
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 4], eax
        jmp    NEAR PTR loc_3_0778
loc_3_06D7:
        or     ebx, ebx
        jne    SHORT loc_3_06E0
        jmp    NEAR PTR loc_3_0768
loc_3_06E0:
        cmp    ebx, 2
        mov    eax, dword ptr [esp + 10h]
        jne    SHORT loc_3_06EE
        add    eax, 20h
        jmp    SHORT loc_3_06F1
loc_3_06EE:
        add    eax, 10h
loc_3_06F1:
        push   dword ptr [esp + 44h]
        push   eax
        call   dword ptr [esp + 48h]
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_0756
        cmp    ebx, 2
        jne    SHORT loc_3_0739
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 0ch]
        add    eax, 20h
        push   eax
        mov    eax, dword ptr [esp + 18h]
        add    eax, 10h
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [eax + 4], 1
loc_3_0739:
        db  0A1h, 050h, 002h, 000h, 000h
        cmp    dword ptr [esp + 10h], eax
        jne    SHORT loc_3_0778
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 8]
        add    eax, 10h
        push   eax
        call   sub_3_1720
        jmp    SHORT loc_3_0778
loc_3_0756:
        cmp    ebx, 1
        jne    SHORT loc_3_0768
        push   0
        push   1
        push   dword ptr [esp + 18h]
        db  0E8h, 098h, 0F9h, 0FCh, 0FFh
loc_3_0768:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], 0
loc_3_0773:
        mov    esi, 51fh
loc_3_0778:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret    0ch
SECPROV_Service_01:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 50h]
        call   sub_3_0135
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_3_07B8
        jmp    NEAR PTR loc_3_08EB
loc_3_07B8:
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        lea    eax, [esp + 20h]
        add    esp, 8
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_3_01B8
        or     eax, eax
        jne    SHORT loc_3_07FC
        mov    ecx, dword ptr [esp + 54h]
        mov    ebx, dword ptr [esp + 50h]
        mov    dword ptr [ecx], 4
        mov    dword ptr [ebx], 0
        mov    dword ptr [esp + 10h], 525h
        jmp    NEAR PTR loc_3_08D9
loc_3_07FC:
        mov    eax, dword ptr [esp + 18h]
        imul   eax, eax, 0ch
        db  08Bh, 00Dh, 050h, 000h, 000h, 000h
        mov    ebp, dword ptr [eax + ecx + 8]
        mov    ecx, dword ptr [esp + 54h]
        mov    esi, dword ptr [ebp]
        or     esi, esi
        jne    SHORT loc_3_082D
        mov    ebx, dword ptr [esp + 50h]
        mov    dword ptr [ecx], 4
        mov    dword ptr [ebx], 0
        jmp    NEAR PTR loc_3_08D9
loc_3_082D:
        mov    eax, dword ptr [ecx]
        sub    eax, 4
        dec    esi
        js     SHORT loc_3_0844
loc_3_0835:
        mov    edx, dword ptr [ebp + esi*4 + 4]
        lea    edx, [edx + esi*4 + 4]
        cmp    edx, eax
        jbe    SHORT loc_3_0844
        dec    esi
        jns    SHORT loc_3_0835
loc_3_0844:
        or     esi, esi
        jge    SHORT loc_3_0862
        mov    ebx, dword ptr [esp + 50h]
        mov    dword ptr [ecx], 4
        mov    dword ptr [ebx], 0
        mov    dword ptr [esp + 10h], 0eah
        jmp    SHORT loc_3_08D9
loc_3_0862:
        mov    ebx, dword ptr [esp + 50h]
        lea    eax, [esi + 1]
        mov    dword ptr [ebx], eax
        lea    eax, [eax*4 + 4]
        add    eax, dword ptr [ebp + esi*4 + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        cmp    eax, 1
        jle    SHORT loc_3_0897
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ebp + 4]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
loc_3_0897:
        lea    edi, [ebx + 4]
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [ebx]
        lea    eax, [ebx + eax*4 + 4]
        mov    dword ptr [esp + 14h], eax
        push   dword ptr [ebp + esi*4 + 4]
        mov    eax, dword ptr [ebp]
        lea    eax, [ebp + eax*4]
        add    eax, 4
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_3_18B0
        add    esp, 0ch
        sub    ecx, ecx
        cmp    dword ptr [ebx], ecx
        jle    SHORT loc_3_08D9
        mov    eax, dword ptr [esp + 14h]
loc_3_08CF:
        add    dword ptr [edi], eax
        add    edi, 4
        inc    ecx
        cmp    dword ptr [ebx], ecx
        jg     SHORT loc_3_08CF
loc_3_08D9:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
loc_3_08EB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
sub_3_08F3:
        push   esi
        push   edi
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        mov    edi, dword ptr [esp + 18h]
        add    esp, 8
        db  068h, 024h, 000h, 000h, 000h
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   dword ptr [esp + 18h]
        call   sub_3_005C
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_3_094D
        call   sub_3_1880
        db  0A3h, 028h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + 4]
        push   dword ptr [eax]
        push   dword ptr [edi]
        db  068h, 02Ch, 000h, 000h, 000h
        call   sub_3_18B0
        add    esp, 0ch
        mov    eax, dword ptr [edi + 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 04Ch, 000h, 000h, 000h
loc_3_094D:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret    8
SECPROV_Service_02:
        sub    esp, 8
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp]
        db  083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        push   eax
        je     SHORT loc_3_0994
        db  068h, 0F3h, 008h, 000h, 000h
        db  068h, 054h, 002h, 000h, 000h
        call   sub_3_04FC
        jmp    SHORT loc_3_09A9
loc_3_0994:
        db  068h, 054h, 002h, 000h, 000h
        call   sub_3_08F3
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 51fh
loc_3_09A9:
        add    esp, 8
        ret
loc_3_09AD:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 14h], eax
        push   dword ptr [ecx]
        call   sub_3_006E
        add    esp, 4
        ret    8
SECPROV_Service_06:
        sub    esp, 80h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 94h]
        or     ebp, ebp
        je     SHORT loc_3_09E7
        cmp    byte ptr [ebp], 0
        mov    esi, 0
        jne    SHORT loc_3_09EC
loc_3_09E7:
        mov    esi, 1
loc_3_09EC:
        mov    eax, dword ptr [esp + 0a4h]
        or     eax, eax
        je     SHORT loc_3_0A01
        cmp    byte ptr [eax], 0
        mov    edi, 0
        jne    SHORT loc_3_0A06
loc_3_0A01:
        mov    edi, 1
loc_3_0A06:
        mov    dword ptr [esp + 1ch], 0
        mov    ecx, dword ptr [esp + 0c0h]
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [ecx], 0
        je     SHORT loc_3_0A2C
        mov    eax, 57h
        jmp    NEAR PTR loc_3_0CF7
loc_3_0A2C:
        mov    ebx, dword ptr [esp + 9ch]
        or     ebx, ebx
        je     NEAR PTR loc_3_0CE9
        cmp    byte ptr [ebx], 0
        je     NEAR PTR loc_3_0CE9
        or     edi, edi
        jne    SHORT loc_3_0A53
        lea    ecx, [esp + 24h]
        push   ecx
        push   eax
        call   sub_3_00B0
loc_3_0A53:
        or     esi, esi
        jne    SHORT loc_3_0A6C
        or     edi, edi
        jne    SHORT loc_3_0A71
        lea    eax, [esp + 24h]
        push   1
        push   eax
        push   ebp
        call   sub_3_00A4
        or     eax, eax
        jne    SHORT loc_3_0A71
loc_3_0A6C:
        db  0BDh, 054h, 002h, 000h, 000h
loc_3_0A71:
        lea    eax, [esp + 68h]
        push   eax
        push   ebx
        push   ebp
        call   sub_3_0135
        or     eax, eax
        je     SHORT loc_3_0A8F
        cmp    eax, 57h
        jne    NEAR PTR loc_3_0CF7
        jmp    NEAR PTR loc_3_0CE9
loc_3_0A8F:
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        add    esp, 8
        db  083h, 03Dh, 024h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0AAD
        jmp    NEAR PTR loc_3_0CD2
loc_3_0AAD:
        mov    eax, dword ptr [esp + 0bch]
        db  039h, 005h, 04Ch, 000h, 000h, 000h
        jne    NEAR PTR loc_3_0CD2
        db  0BEh, 02Ch, 000h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, dword ptr [esp + 0b8h]
        repe cmpsd
        jne    NEAR PTR loc_3_0CD2
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_3_0CD2
        db  083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_0B17
        push   1
        db  068h, 054h, 002h, 000h, 000h
        push   ebp
        call   sub_3_00A4
        or     eax, eax
        je     SHORT loc_3_0B17
        mov    dword ptr [esp + 1ch], 1
        lea    eax, [esp + 18h]
        push   eax
        push   2
        push   0
        push   ebp
        call   sub_3_1254
loc_3_0B17:
        mov    word ptr [esp + 12h], 0
        mov    word ptr [esp + 14h], 0
        lea    eax, [esp + 14h]
        mov    dword ptr [esp + 4ch], eax
        lea    eax, [esp + 12h]
        mov    dword ptr [esp + 50h], eax
        db  083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        mov    word ptr [esp + 54h], 834h
        db  0C7h, 044h, 024h, 056h, 05Ch, 000h, 000h, 000h
        mov    word ptr [esp + 5ah], 0
        mov    dword ptr [esp + 5ch], ebx
        je     SHORT loc_3_0B66
        db  0A1h, 050h, 002h, 000h, 000h
        add    eax, 10h
        mov    dword ptr [esp + 60h], eax
        jmp    SHORT loc_3_0B6E
loc_3_0B66:
        db  0C7h, 044h, 024h, 060h, 054h, 002h, 000h, 000h
loc_3_0B6E:
        mov    dword ptr [esp + 64h], ebp
        lea    eax, [esp + 4ch]
        push   eax
        push   dword ptr [esp + 0b4h]
        push   dword ptr [esp + 0b0h]
        db  0FFh, 035h, 024h, 000h, 000h, 000h
        call   sub_3_0068
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_0BB4
        cmp    esi, 5
        je     SHORT loc_3_0BB4
        cmp    esi, 8
        je     SHORT loc_3_0BC2
        cmp    esi, 0eah
        je     SHORT loc_3_0BB2
        mov    esi, 51fh
        jmp    SHORT loc_3_0BC2
loc_3_0BB2:
        sub    esi, esi
loc_3_0BB4:
        mov    eax, dword ptr [esp + 0c4h]
        mov    word ptr [eax], si
        sub    esi, esi
        jmp    SHORT loc_3_0BC9
loc_3_0BC2:
        mov    eax, dword ptr [esp + 0c4h]
loc_3_0BC9:
        or     esi, esi
        jne    NEAR PTR loc_3_0CB5
        cmp    word ptr [eax], 0
        jne    NEAR PTR loc_3_0CB5
        mov    di, word ptr [esp + 12h]
        cmp    dword ptr [esp + 1ch], 0
        mov    ax, word ptr [esp + 14h]
        mov    word ptr [esp + 16h], ax
        je     SHORT loc_3_0C5C
        mov    word ptr [esp + 12h], 0
        mov    word ptr [esp + 14h], 0
        lea    eax, [esp + 14h]
        mov    dword ptr [esp + 34h], eax
        lea    eax, [esp + 12h]
        mov    dword ptr [esp + 38h], eax
        mov    word ptr [esp + 3ch], 834h
        db  0C7h, 044h, 024h, 03Eh, 05Ch, 000h, 000h, 000h
        mov    word ptr [esp + 42h], 0
        lea    eax, [esp + 34h]
        mov    dword ptr [esp + 44h], ebx
        mov    dword ptr [esp + 20h], eax
        lea    eax, [esp + 20h]
        push   eax
        db  068h, 0ADh, 009h, 000h, 000h
        push   ebp
        call   sub_3_04FC
        or     eax, eax
        je     SHORT loc_3_0C5C
        cmp    word ptr [esp + 14h], 0
        jne    SHORT loc_3_0C5C
        mov    ax, word ptr [esp + 16h]
        mov    word ptr [esp + 14h], ax
        mov    word ptr [esp + 12h], di
loc_3_0C5C:
        db  068h, 05Ch, 000h, 000h, 000h
        lea    eax, [esp + 6ch]
        push   dword ptr [esp + 16h]
        push   eax
        push   ebp
        call   sub_3_0217
        mov    ecx, dword ptr [esp + 0c0h]
        mov    eax, dword ptr [esp + 98h]
        mov    dword ptr [ecx], 0
        cmp    eax, ebp
        je     SHORT loc_3_0C98
        or     eax, eax
        je     SHORT loc_3_0C98
        push   -1
        push   ebp
        push   eax
        call   sub_3_18A0
        add    esp, 0ch
loc_3_0C98:
        mov    eax, dword ptr [esp + 0a0h]
        cmp    eax, ebx
        je     SHORT loc_3_0CD7
        or     eax, eax
        je     SHORT loc_3_0CD7
        push   -1
        push   ebx
        push   eax
        call   sub_3_18A0
        add    esp, 0ch
        jmp    SHORT loc_3_0CD7
loc_3_0CB5:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_3_0CD7
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_3_0CD7
        push   0
        push   1
        push   dword ptr [esp + 20h]
        db  0E8h, 030h, 0F4h, 0FCh, 0FFh
        jmp    SHORT loc_3_0CD7
loc_3_0CD2:
        mov    esi, 57h
loc_3_0CD7:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, esi
        jmp    SHORT loc_3_0CF7
loc_3_0CE9:
        mov    eax, dword ptr [esp + 0c4h]
        mov    word ptr [eax], 5
        sub    eax, eax
loc_3_0CF7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 80h
        ret
SECPROV_Service_03:
        sub    esp, 4
        mov    dword ptr [esp], 0
        lea    eax, [esp]
        push   dword ptr [esp + 24h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        push   0
        push   0
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        push   0
        push   dword ptr [esp + 34h]
        push   0
        push   dword ptr [esp + 38h]
        call   SECPROV_Service_06
        add    esp, 38h
        ret
SECPROV_Service_04:
        sub    esp, 2ch
        push   esi
        mov    eax, dword ptr [esp + 38h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_0D5A
        mov    eax, 57h
        jmp    NEAR PTR loc_3_0E04
loc_3_0D5A:
        lea    ecx, [esp + 8]
        push   ecx
        push   eax
        push   dword ptr [esp + 3ch]
        call   sub_3_0135
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_0D74
        jmp    NEAR PTR loc_3_0E02
loc_3_0D74:
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        lea    eax, [esp + 0ch]
        add    esp, 8
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_3_01B8
        or     eax, eax
        jne    SHORT loc_3_0D9E
        mov    esi, 525h
        jmp    SHORT loc_3_0DF4
loc_3_0D9E:
        mov    ecx, dword ptr [esp + 4]
        imul   ecx, ecx, 0ch
        db  003h, 00Dh, 050h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx + 4]
        dec    eax
        mov    dword ptr [ecx + 4], eax
        jne    SHORT loc_3_0DBA
        push   ecx
        call   sub_3_045D
loc_3_0DBA:
        db  083h, 03Dh, 058h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0DF4
        call   sub_3_1880
        db  03Bh, 005h, 028h, 000h, 000h, 000h
        jbe    SHORT loc_3_0DF4
        db  02Bh, 005h, 028h, 000h, 000h, 000h
        cmp    eax, 0ea60h
        jb     SHORT loc_3_0DF4
        db  068h, 024h, 000h, 000h, 000h
        call   sub_3_0062
        add    esp, 4
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_0DF4:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
loc_3_0E02:
        mov    eax, esi
loc_3_0E04:
        pop    esi
        add    esp, 2ch
        ret
SECPROV_Service_05:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 04Ch, 002h, 000h, 000h
        mov    dword ptr [ecx], eax
        db  0A1h, 000h, 009h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  0C7h, 041h, 008h, 054h, 002h, 000h, 000h
        db  083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_0E36
        db  0A1h, 050h, 002h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        jmp    SHORT loc_3_0E3B
loc_3_0E36:
        db  0A1h, 000h, 009h, 000h, 000h
loc_3_0E3B:
        mov    dword ptr [ecx + 0ch], eax
        db  083h, 03Dh, 048h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_0E54
        db  0A1h, 050h, 002h, 000h, 000h
        add    eax, 10h
        mov    dword ptr [ecx + 10h], eax
        jmp    SHORT loc_3_0E5B
loc_3_0E54:
        db  0C7h, 041h, 010h, 054h, 002h, 000h, 000h
loc_3_0E5B:
        db  0A1h, 008h, 00Ah, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0C7h, 041h, 018h, 004h, 009h, 000h, 000h
        sub    eax, eax
        ret
loc_3_0E6D:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   SECPROV_Service_02
        add    esp, 8
        ret
loc_3_0E7F:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, 32h
        cmp    dword ptr [esi + 1ch], 2
        jne    SHORT loc_3_0EBD
        lea    eax, [esp + 6]
        push   eax
        push   dword ptr [esi + 18h]
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   SECPROV_Service_03
        movzx  ecx, word ptr [esp + 26h]
        add    esp, 20h
        mov    edx, dword ptr [esi + 20h]
        mov    dword ptr [edx], ecx
loc_3_0EBD:
        pop    esi
        add    esp, 4
        ret
loc_3_0EC2:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 6]
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 28h]
        push   dword ptr [esi + 24h]
        push   dword ptr [esi + 20h]
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 18h]
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   SECPROV_Service_06
        movzx  ecx, word ptr [esp + 3ah]
        add    esp, 34h
        mov    edx, dword ptr [esi + 30h]
        pop    esi
        mov    dword ptr [edx], ecx
        add    esp, 4
        ret
loc_3_0F09:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   SECPROV_Service_04
        add    esp, 0ch
        ret
loc_3_0F1E:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx], 2
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 00Dh, 04Ch, 002h, 000h, 000h
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret
loc_3_0F38:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        db  08Bh, 00Dh, 000h, 009h, 000h, 000h
        cmp    dword ptr [eax], ecx
        mov    eax, 6fh
        jb     SHORT loc_3_0F70
        db  0FFh, 035h, 000h, 009h, 000h, 000h
        db  068h, 054h, 002h, 000h, 000h
        push   dword ptr [esi]
        call   sub_3_18B0
        add    esp, 0ch
        mov    ecx, dword ptr [esi + 4]
        db  0A1h, 000h, 009h, 000h, 000h
        mov    dword ptr [ecx], eax
        sub    eax, eax
loc_3_0F70:
        pop    esi
        ret
loc_3_0F72:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        lea    eax, [esp + 10h]
        add    esp, 8
        push   eax
        push   2
        mov    esi, dword ptr [esp + 18h]
        push   0
        push   dword ptr [esi]
        call   sub_3_1254
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_0FB1
        cmp    edi, 4d5h
        jne    SHORT loc_3_0FBE
loc_3_0FB1:
        push   0
        push   1
        push   dword ptr [esp + 10h]
        db  0E8h, 042h, 0F1h, 0FCh, 0FFh
loc_3_0FBE:
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_3_103D
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_3_0FD7
        mov    edi, 51fh
        jmp    SHORT loc_3_103D
loc_3_0FD7:
        mov    ecx, dword ptr [esp + 8]
        mov    ecx, dword ptr [ecx + 8]
        inc    ecx
        cmp    eax, 2
        jb     SHORT loc_3_0FEB
        mov    eax, dword ptr [esp + 8]
        add    ecx, dword ptr [eax + 0ch]
loc_3_0FEB:
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax], ecx
        jae    SHORT loc_3_0FFB
        mov    dword ptr [eax], ecx
        mov    edi, 6fh
        jmp    SHORT loc_3_103D
loc_3_0FFB:
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        add    eax, 10h
        push   eax
        push   esi
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 14h]
        add    esp, 0ch
        add    esi, dword ptr [eax + 8]
        cmp    dword ptr [eax + 4], 2
        jb     SHORT loc_3_1038
        push   dword ptr [eax + 0ch]
        add    eax, 20h
        push   eax
        push   esi
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 14h]
        add    esp, 0ch
        add    esi, dword ptr [eax + 0ch]
loc_3_1038:
        mov    byte ptr [esi], 0
        sub    edi, edi
loc_3_103D:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_3_1053:
        mov    eax, 1
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_1060:
        push   esi
        push   edi
        db  08Bh, 03Dh, 098h, 008h, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_1086
        mov    esi, dword ptr [esp + 0ch]
loc_3_1070:
        push   1
        lea    eax, [edi + 4]
        push   eax
        push   esi
        call   sub_3_00A4
        or     eax, eax
        je     SHORT loc_3_1086
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_3_1070
loc_3_1086:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
        db  0FFh, 074h, 024h, 004h, 0E8h, 0CAh, 0FFh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 005h, 08Bh, 040h, 018h
        db  0EBh, 002h, 02Bh, 0C0h, 0C2h, 004h, 000h
sub_3_10A4:
        push   esi
        push   0
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        mov    esi, dword ptr [esp + 10h]
        add    esp, 8
        mov    eax, dword ptr [esi + 24h]
        or     eax, eax
        je     SHORT loc_3_10CD
        push   eax
        call   sub_3_1870
        mov    dword ptr [esi + 24h], 0
loc_3_10CD:
        call   sub_3_1880
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [eax], 0
        push   dword ptr [esi + 18h]
        call   sub_3_1898
        add    esp, 4
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        pop    esi
        ret    4
sub_3_10FB:
        push   0
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        add    esp, 8
        db  08Bh, 00Dh, 098h, 008h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_112E
        mov    eax, dword ptr [esp + 4]
loc_3_1119:
        cmp    dword ptr [ecx + 18h], eax
        je     SHORT loc_3_1124
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_3_1119
loc_3_1124:
        or     ecx, ecx
        je     SHORT loc_3_112E
        push   ecx
        call   sub_3_10A4
loc_3_112E:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        ret    4
sub_3_113F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 18h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_3_11A0
        cmp    dword ptr [esi + 28h], 3
        je     SHORT loc_3_1193
        cmp    dword ptr [eax + 4], 2
        je     SHORT loc_3_1193
        push   esi
        push   1388h
        db  068h, 0A9h, 015h, 000h, 000h
        call   sub_3_1854
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esi + 28h]
        inc    eax
        mov    dword ptr [esi + 28h], eax
        mov    word ptr [esi + 36h], ax
        push   1ch
        push   dword ptr [esi + 30h]
        lea    eax, [esi + 34h]
        push   eax
        push   dword ptr [esi + 14h]
        add    esi, 197h
        push   esi
        call   sub_3_0056
        add    esp, 14h
        jmp    SHORT loc_3_11A0
loc_3_1193:
        mov    dword ptr [esi + 24h], 0
        push   esi
        call   sub_3_10A4
loc_3_11A0:
        pop    esi
        ret    4
sub_3_11A4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 18h]
        cmp    dword ptr [edi], 0
        je     NEAR PTR loc_3_123D
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        jne    SHORT loc_3_11F8
        lea    ebx, [esi + 4]
        push   dword ptr [esi + 14h]
        push   ebx
        lea    eax, [edi + 10h]
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [edi + 8], eax
        inc    dword ptr [edi + 4]
        push   edi
        call   sub_3_1898
        add    esp, 4
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 2ch], 1
        je     SHORT loc_3_1230
        push   dword ptr [esi + 14h]
        push   ebx
        call   sub_3_1720
        jmp    SHORT loc_3_1230
loc_3_11F8:
        cmp    eax, 1
        jne    SHORT loc_3_1230
        lea    ebx, [esi + 4]
        push   ebx
        lea    eax, [edi + 10h]
        push   eax
        db  0E8h, 061h, 0EFh, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_3_1230
        push   dword ptr [esi + 14h]
        push   ebx
        lea    eax, [edi + 20h]
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [edi + 0ch], eax
        inc    dword ptr [edi + 4]
        push   edi
        call   sub_3_1898
        add    esp, 4
loc_3_1230:
        cmp    dword ptr [edi + 4], 2
        jne    SHORT loc_3_123D
        push   dword ptr [esi]
        call   sub_3_10A4
loc_3_123D:
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_3_124E
        push   0
        push   esi
        call   sub_3_1844
        add    esp, 8
loc_3_124E:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_1254:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   1
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1888
        add    esp, 8
        db  083h, 03Dh, 090h, 008h, 000h, 000h, 000h
        je     NEAR PTR loc_3_1328
        db  0C7h, 005h, 090h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 094h, 008h, 000h, 000h, 010h, 002h, 000h, 000h
        db  0A1h, 094h, 008h, 000h, 000h
        db  066h, 0C7h, 005h, 014h, 002h, 000h, 000h, 00Ah, 000h
        movzx  eax, word ptr [eax + 4]
        push   eax
        db  068h, 09Ch, 008h, 000h, 000h
        db  0A1h, 094h, 008h, 000h, 000h
        add    eax, 6
        push   eax
        call   sub_3_18B0
        db  0A1h, 094h, 008h, 000h, 000h
        add    esp, 0ch
        mov    word ptr [eax + 2], 4
        db  0A1h, 094h, 008h, 000h, 000h
        mov    cx, word ptr [eax + 2]
        add    cx, word ptr [eax + 4]
        mov    word ptr [eax], cx
        call   sub_3_0074
        mov    esi, eax
        push   dword ptr [esi + 4]
        call   sub_3_18A8
        add    esp, 4
        inc    eax
        db  0A3h, 044h, 002h, 000h, 000h
        push   1
        push   eax
        push   dword ptr [esi + 4]
        db  068h, 024h, 002h, 000h, 000h
        call   sub_3_0086
        add    esp, 10h
        db  0A3h, 044h, 002h, 000h, 000h
        push   eax
        db  068h, 024h, 002h, 000h, 000h
        db  068h, 024h, 002h, 000h, 000h
        call   sub_3_0092
        add    esp, 0ch
        db  068h, 020h, 002h, 000h, 000h
        db  0FFh, 035h, 094h, 008h, 000h, 000h
        db  068h, 0DFh, 015h, 000h, 000h
        call   sub_3_007A
        add    esp, 0ch
loc_3_1328:
        mov    edi, dword ptr [esp + 14h]
        push   edi
        call   sub_3_1060
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_3_142D
        push   101h
        push   1beh
        call   sub_3_183C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_1378
        push   1
        push   30h
        call   sub_3_183C
        add    esp, 8
        mov    dword ptr [esi + 18h], eax
        or     eax, eax
        jne    SHORT loc_3_1374
        push   0
        push   esi
        call   sub_3_1844
        add    esp, 8
        sub    esi, esi
loc_3_1374:
        or     esi, esi
        jne    SHORT loc_3_1390
loc_3_1378:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, 8
        jmp    NEAR PTR loc_3_15A2
loc_3_1390:
        push   edi
        call   sub_3_18A8
        add    esp, 4
        mov    dword ptr [esi + 14h], eax
        inc    eax
        push   eax
        push   edi
        lea    eax, [esi + 4]
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [eax + 0ch], 1
        lea    eax, [esi + 199h]
        mov    byte ptr [esi + 198h], 5ch
        mov    byte ptr [esi + 197h], 5ch
        push   dword ptr [esi + 14h]
        push   edi
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        push   16h
        db  068h, 0A8h, 008h, 000h, 000h
        mov    eax, dword ptr [esi + 14h]
        add    eax, esi
        add    eax, 199h
        push   eax
        call   sub_3_18B0
        add    esp, 0ch
        db  083h, 03Dh, 098h, 008h, 000h, 000h, 000h
        jne    SHORT loc_3_1417
        db  089h, 035h, 098h, 008h, 000h, 000h
        mov    eax, dword ptr [esi + 18h]
        db  0A3h, 050h, 002h, 000h, 000h
        mov    edi, dword ptr [esp + 1ch]
        jmp    SHORT loc_3_1488
loc_3_1417:
        db  0A1h, 098h, 008h, 000h, 000h
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        db  0A1h, 098h, 008h, 000h, 000h
        mov    dword ptr [eax], esi
        jmp    SHORT loc_3_1488
loc_3_142D:
        mov    edi, dword ptr [esp + 1ch]
        test   edi, 2
        je     SHORT loc_3_1463
        mov    eax, dword ptr [esi + 18h]
        cmp    dword ptr [eax + 4], 0
        jbe    SHORT loc_3_1463
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx], eax
        mov    eax, 0b7h
        jmp    NEAR PTR loc_3_15A2
loc_3_1463:
        mov    eax, dword ptr [esi + 18h]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_3_1586
        mov    ebx, dword ptr [esi + 20h]
        or     ebx, ebx
        je     SHORT loc_3_1488
        call   sub_3_1880
        sub    eax, ebx
        cmp    eax, 0afc8h
        jb     NEAR PTR loc_3_1586
loc_3_1488:
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [eax + 4], 0
        call   sub_3_18B8
        mov    dword ptr [esi + 1ch], eax
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    word ptr [esi + 34h], 12h
        lea    ebx, [esi + 38h]
        db  0FFh, 035h, 044h, 002h, 000h, 000h
        db  068h, 024h, 002h, 000h, 000h
        push   ebx
        call   sub_3_18B0
        add    esp, 0ch
        db  0A1h, 044h, 002h, 000h, 000h
        and    eax, 0fffffffeh
        add    ebx, eax
        mov    ebp, dword ptr [esp + 18h]
        or     ebp, ebp
        je     SHORT loc_3_14FB
        push   ebp
        call   sub_3_18A8
        add    esp, 4
        inc    eax
        push   1
        push   eax
        push   ebp
        push   ebx
        call   sub_3_0086
        add    esp, 10h
        mov    ebp, eax
        jmp    SHORT loc_3_150F
loc_3_14FB:
        mov    ebp, 1ch
        push   ebp
        db  068h, 0C0h, 008h, 000h, 000h
        push   ebx
        call   sub_3_18B0
        add    esp, 0ch
loc_3_150F:
        shr    ebp, 1
        lea    ebx, [ebx + ebp*2]
        db  068h, 0DCh, 008h, 000h, 000h
        call   sub_3_18A8
        add    esp, 4
        lea    ebp, [eax + 1]
        push   ebp
        db  068h, 0ECh, 008h, 000h, 000h
        push   ebx
        call   sub_3_18B0
        add    esp, 0ch
        lea    eax, [ebx + ebp]
        mov    dword ptr [eax], 10h
        add    eax, 4
        mov    dword ptr [eax], 0
        add    eax, 4
        mov    dword ptr [eax], 1
        add    eax, 4
        mov    word ptr [eax], 0ffffh
        add    eax, 2
        mov    word ptr [eax], 0ffffh
        add    eax, 2
        sub    eax, esi
        sub    eax, 34h
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi + 2ch], edi
        push   esi
        call   sub_3_113F
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    SHORT loc_3_15A2
loc_3_1586:
        db  0FFh, 035h, 020h, 000h, 000h, 000h
        call   sub_3_1890
        add    esp, 4
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ecx], eax
        mov    eax, 4d5h
loc_3_15A2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
loc_3_15A9:
        sub    esp, 4
        mov    dword ptr [esp], edx
        call   sub_3_18B8
        mov    ecx, dword ptr [esp]
        cmp    eax, dword ptr [ecx + 1ch]
        jne    SHORT loc_3_15C6
        push   ecx
        call   sub_3_113F
        jmp    SHORT loc_3_15DB
loc_3_15C6:
        push   dword ptr [esp]
        mov    eax, dword ptr [esp + 4]
        db  068h, 09Dh, 000h, 000h, 000h
        push   dword ptr [eax + 1ch]
        db  0E8h, 0DAh, 0EAh, 0FCh, 0FFh
loc_3_15DB:
        add    esp, 4
        ret
loc_3_15DF:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 3ch]
        mov    ax, word ptr [esi]
        cmp    ax, 13h
        je     SHORT loc_3_1602
        cmp    ax, 15h
        je     SHORT loc_3_1602
        mov    eax, 1
        jmp    NEAR PTR loc_3_1718
loc_3_1602:
        sub    ecx, ecx
        cmp    word ptr [esi + 2], cx
        lea    eax, [esi + 2]
        je     SHORT loc_3_1617
loc_3_160D:
        add    eax, 2
        inc    ecx
        cmp    word ptr [eax], 0
        jne    SHORT loc_3_160D
loc_3_1617:
        lea    edi, [ecx*2 - 4]
        lea    ebx, [esi + 6]
        push   edi
        push   ebx
        push   ebx
        call   sub_3_0092
        add    esp, 0ch
        push   1
        push   0fh
        push   edi
        lea    eax, [esp + 2ch]
        push   ebx
        push   eax
        call   sub_3_008C
        mov    dword ptr [esp + 44h], eax
        add    esp, 14h
        mov    byte ptr [esp + eax + 20h], 0
        inc    dword ptr [esp + 30h]
        lea    edx, [esi + edi + 8]
loc_3_1650:
        mov    eax, edx
        add    edx, 2
        cmp    word ptr [eax], 0
        jne    SHORT loc_3_1650
        sub    ecx, ecx
        cmp    word ptr [edx], cx
        je     SHORT loc_3_166E
        mov    eax, edx
loc_3_1664:
        add    eax, 2
        inc    ecx
        cmp    word ptr [eax], 0
        jne    SHORT loc_3_1664
loc_3_166E:
        push   1
        push   0fh
        lea    eax, [ecx*2]
        push   eax
        lea    eax, [esp + 18h]
        push   edx
        push   eax
        call   sub_3_008C
        add    esp, 14h
        mov    byte ptr [esp + eax + 0ch], 0
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_3_1060
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_3_16A6
        mov    eax, 2
        jmp    SHORT loc_3_1718
loc_3_16A6:
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + 18h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_16B4
        jmp    SHORT loc_3_1716
loc_3_16B4:
        call   sub_3_18B8
        mov    ecx, dword ptr [esp + 1ch]
        sub    eax, dword ptr [ecx + 1ch]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 34h], eax
        or     eax, eax
        jne    SHORT loc_3_16DA
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_3_11A4
        jmp    SHORT loc_3_1716
loc_3_16DA:
        push   100h
        push   1ch
        call   sub_3_183C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        mov    eax, 8
        je     SHORT loc_3_1718
        lea    eax, [esp + 1ch]
        push   1ch
        push   eax
        push   esi
        call   sub_3_18B0
        add    esp, 0ch
        push   esi
        mov    eax, dword ptr [esp + 20h]
        db  068h, 0A9h, 000h, 000h, 000h
        push   dword ptr [eax + 1ch]
        db  0E8h, 09Fh, 0E9h, 0FCh, 0FFh
loc_3_1716:
        sub    eax, eax
loc_3_1718:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
        db  000h
sub_3_1720:
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  0A3h, 03Ch, 00Ah, 000h, 000h
        push   dword ptr [esp + 8]
        db  068h, 02Ch, 00Ah, 000h, 000h
        call   sub_3_18B0
        add    esp, 0ch
        push   0
        db  068h, 054h, 017h, 000h, 000h
        push   0
        push   40h
        push   0
        push   0
        db  0E8h, 082h, 0E9h, 0FCh, 0FFh
        sub    eax, eax
        ret    8
loc_3_1754:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        db  068h, 040h, 00Ah, 000h, 000h
        push   80000002h
        call   sub_3_1824
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_179E
        db  0FFh, 035h, 03Ch, 00Ah, 000h, 000h
        db  068h, 02Ch, 00Ah, 000h, 000h
        push   1
        push   0
        db  068h, 054h, 00Ah, 000h, 000h
        push   dword ptr [esp + 14h]
        call   sub_3_1834
        add    esp, 18h
        push   dword ptr [esp]
        call   sub_3_182C
        add    esp, 4
loc_3_179E:
        add    esp, 4
        ret
loc_3_17A2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [esp + 10h], 0
        mov    ebx, dword ptr [esi]
        mov    dword ptr [esi], 0
        mov    edi, dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 20h]
        or     ebp, ebp
        je     SHORT loc_3_1810
loc_3_17C9:
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        add    dword ptr [esi], eax
        mov    eax, dword ptr [edi]
        cmp    ebx, eax
        jb     SHORT loc_3_17FB
        push   eax
        push   dword ptr [ecx]
        push   dword ptr [esp + 2ch]
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 30h]
        add    esp, 0ch
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [edi]
        add    dword ptr [esp + 24h], eax
        sub    ebx, dword ptr [edi]
        jmp    SHORT loc_3_180A
loc_3_17FB:
        mov    dword ptr [edi + 8], 0
        mov    dword ptr [esp + 10h], 0eah
loc_3_180A:
        add    edi, 0ch
        dec    ebp
        jne    SHORT loc_3_17C9
loc_3_1810:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h
sub_3_1824:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_182C:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_1834:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_183C:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_1844:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_184C:
        db  0CDh, 020h, 050h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_1854:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_3_1870:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_3_1880:
        db  0CDh, 020h, 0CFh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_3_1888:
        db  0CDh, 020h, 02Eh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_1890:
        db  0CDh, 020h, 02Fh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_1898:
        db  0CDh, 020h, 027h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_18A0:
        db  0CDh, 020h, 07Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_18A8:
        db  0CDh, 020h, 07Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_18B0:
        db  0CDh, 020h, 07Dh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_18B8:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
_PTEXT ends
        end
