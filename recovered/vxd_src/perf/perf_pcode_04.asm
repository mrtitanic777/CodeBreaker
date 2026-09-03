; perf_pcode_04.asm -- PERF object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0C7h, 005h, 018h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        clc
        ret
loc_4_000C:
        db  0C7h, 005h, 014h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_4_0BA3
        pop    ecx
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_0480
        pop    ecx
        pop    ecx
        ret
sub_4_0036:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 8]
        mov    edx, dword ptr [edx + 8]
        or     edx, edx
        je     SHORT loc_4_005D
        push   eax
        cmp    ecx, 3
        je     SHORT loc_4_0056
        push   11h
        jmp    SHORT loc_4_0058
loc_4_0056:
        push   12h
loc_4_0058:
        call   edx
        add    esp, 8
loc_4_005D:
        pop    eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_0062:
        push   ebp
        mov    ebp, esp
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 14h], 1
        jne    SHORT loc_4_0076
        mov    eax, dword ptr [ecx + 0ch]
        mov    eax, dword ptr [eax]
        jmp    SHORT loc_4_007B
loc_4_0076:
        push   ecx
        call   dword ptr [ecx + 0ch]
        pop    ecx
loc_4_007B:
        test   dword ptr [ecx + 14h], 40h
        je     SHORT loc_4_008A
        mul    dword ptr [ecx + 18h]
        clc
        jmp    SHORT loc_4_009C
loc_4_008A:
        test   dword ptr [ecx + 14h], 80h
        je     SHORT loc_4_009C
        mov    edx, 0
        div    dword ptr [ecx + 18h]
        clc
loc_4_009C:
        pop    ecx
        pop    ebp
        ret
PERF_Service_00:
        mov    eax, 400h
        clc
        ret
PERF_Service_01:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0ch]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        add    eax, 1eh
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_015A
        mov    edi, eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 4], 0
        lea    eax, [edi + 1ch]
        push   -1
        push   dword ptr [esi + 0ch]
        push   eax
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        lea    eax, [edi + 10h]
        push   eax
        push   dword ptr [esi + 0ch]
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_0156
        push   dword ptr [esi + 8]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        inc    eax
        push   eax
        push   dword ptr [esi + 8]
        push   1
        push   0
        db  068h, 050h, 000h, 000h, 000h
        push   dword ptr [edi + 10h]
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_015A
        lea    eax, [edi + 1ch]
        push   eax
        call   sub_4_0DE6
        pop    ecx
        mov    dword ptr [edi + 18h], eax
        mov    eax, edi
        db  087h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi], eax
        mov    eax, edi
        db  0FFh, 005h, 010h, 000h, 000h, 000h
loc_4_0156:
        pop    edi
        pop    esi
        leave
        ret
loc_4_015A:
        sub    eax, eax
        jmp    SHORT loc_4_0156
PERF_Service_02:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  083h, 03Dh, 018h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_4_01FE
        db  08Dh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_4_0179:
        mov    edx, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_4_01FE
        cmp    edi, esi
        jne    SHORT loc_4_0179
        push   edx
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0190:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     SHORT loc_4_01A3
        push   eax
        db  0CDh, 020h, 004h, 000h, 048h, 000h
        add    esp, 4
        jmp    SHORT loc_4_0190
loc_4_01A3:
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        pop    esi
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        push   dword ptr [edi + 10h]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        lea    eax, [edi + 1ch]
        push   eax
        call   sub_4_0DE6
        pop    ecx
        cmp    eax, 0
        jne    SHORT loc_4_01E0
        lea    eax, [edi + 1ch]
        push   eax
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        add    esp, 8
loc_4_01E0:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_0480
        pop    ecx
        pop    ecx
loc_4_01FE:
        pop    edi
        pop    esi
        leave
        ret
PERF_Service_03:
        enter  8, 0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 18h]
        test   eax, eax
        je     NEAR PTR loc_4_03D7
        push   dword ptr [esi + 0ch]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        inc    eax
        mov    dword ptr [ebp - 4], eax
        add    eax, 21h
        test   dword ptr [esi + 4], 100h
        je     SHORT loc_4_0251
        push   esi
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], 0
loc_4_023B:
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_0249
        add    dword ptr [ebp - 8], 4
        add    esi, 4
        jmp    SHORT loc_4_023B
loc_4_0249:
        pop    esi
        add    dword ptr [ebp - 8], 4
        add    eax, dword ptr [ebp - 8]
loc_4_0251:
        push   0
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_03D7
        mov    edi, eax
        mov    dword ptr [ebp + 0ch], eax
        lea    eax, [edi + 20h]
        push   -1
        push   dword ptr [esi + 0ch]
        push   eax
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [edi + 1ch], 0
        test   dword ptr [esi + 4], 100h
        je     SHORT loc_4_02A4
        lea    eax, [edi + 20h]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 1ch], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [esi + 18h]
        push   eax
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
loc_4_02A4:
        lea    eax, [edi + 10h]
        push   eax
        push   dword ptr [esi + 0ch]
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 10h]
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_03D7
        push   dword ptr [esi + 8]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        inc    eax
        push   eax
        push   dword ptr [esi + 8]
        push   1
        push   0
        db  068h, 05Dh, 000h, 000h, 000h
        push   dword ptr [edi + 10h]
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        or     eax, eax
        jne    NEAR PTR loc_4_03D4
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_4_0325
        push   dword ptr [esi + 14h]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        inc    eax
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [esp + 0ch], 1
        db  0C7h, 044h, 024h, 004h, 062h, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        or     eax, eax
        jne    NEAR PTR loc_4_03D4
loc_4_0325:
        db  0B8h, 081h, 000h, 000h, 000h
        test   byte ptr [esi + 4], 2
        je     SHORT loc_4_0335
        db  0B8h, 07Ch, 000h, 000h, 000h
loc_4_0335:
        mov    dword ptr [esp + 10h], eax
        push   eax
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        pop    ecx
        inc    eax
        mov    dword ptr [esp + 14h], eax
        db  0C7h, 044h, 024h, 004h, 06Eh, 000h, 000h, 000h
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_4_03D4
        add    esp, 18h
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [ecx + 4], edi
        mov    eax, edi
        db  087h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 14h], eax
        sub    eax, eax
        test   dword ptr [esi + 4], 100h
        jne    SHORT loc_4_0385
        mov    eax, dword ptr [esi + 18h]
loc_4_0385:
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [edi + 18h], eax
        test   dword ptr [edi + 14h], 0c0h
        je     SHORT loc_4_03AA
        cmp    dword ptr [edi + 18h], 0
        jne    SHORT loc_4_03AA
        and    dword ptr [edi + 14h], 0ffffff3fh
loc_4_03AA:
        mov    eax, edi
        db  0FFh, 005h, 00Ch, 000h, 000h, 000h
        db  083h, 03Dh, 014h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_03D0
        push   eax
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_0480
        pop    ecx
        pop    ecx
        pop    eax
loc_4_03D0:
        pop    edi
        pop    esi
        leave
        ret
loc_4_03D4:
        add    esp, 18h
loc_4_03D7:
        sub    eax, eax
        jmp    SHORT loc_4_03D0
PERF_Service_04:
        push   ebp
        mov    ebp, esp
        db  083h, 03Dh, 018h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_045E
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 8]
loc_4_03EF:
        mov    ecx, edx
        mov    edx, dword ptr [edx + 4]
        or     edx, edx
        je     SHORT loc_4_0462
        cmp    edx, esi
        jne    SHORT loc_4_03EF
        db  08Dh, 015h, 004h, 000h, 000h, 000h
loc_4_0402:
        mov    ebx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_4_0462
        cmp    edx, esi
        jne    SHORT loc_4_0402
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx], eax
        push   dword ptr [esi + 10h]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        lea    eax, [esi + 20h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 10h]
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        add    esp, 8
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  083h, 03Dh, 01Ch, 000h, 000h, 000h, 001h
        jne    SHORT loc_4_045E
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_4_0480
        pop    ecx
        pop    ecx
loc_4_045E:
        pop    ebx
        pop    esi
        leave
        ret
loc_4_0462:
        mov    eax, 6
        stc
        jmp    SHORT loc_4_045E
        db  0C3h
loc_4_046B:
        push   edx
        call   sub_4_0A68
        pop    ecx
        ret
loc_4_0473:
        push   edx
        call   sub_4_0AA1
        pop    ecx
        ret
        db  000h, 000h, 000h, 000h, 000h
sub_4_0480:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 40h]
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 10h], 0
        call   sub_4_081E
        push   0
        db  0A1h, 00Ch, 000h, 000h, 000h
        shl    eax, 4
        push   eax
        db  0E8h, 0F8h, 0FBh, 0FBh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        push   0
        db  0A1h, 00Ch, 000h, 000h, 000h
        imul   eax, eax, 66h
        push   eax
        db  0E8h, 0E1h, 0FBh, 0FBh, 0FFh
        mov    dword ptr [esp + 28h], eax
        add    esp, 8
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_4_07FF
        or     eax, eax
        je     NEAR PTR loc_4_07EA
        mov    ebp, eax
        or     esi, esi
        je     SHORT loc_4_0542
        mov    ebx, dword ptr [esp + 14h]
loc_4_04EB:
        mov    eax, dword ptr [esp + 18h]
        db  039h, 005h, 00Ch, 000h, 000h, 000h
        jbe    SHORT loc_4_0542
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     NEAR PTR loc_4_0814
        push   ebp
        lea    ecx, [esi + 20h]
        push   ecx
        add    eax, 1ch
        push   eax
        call   sub_4_0AD9
        add    esp, 0ch
        mov    dword ptr [ebx], ebp
        mov    dword ptr [ebx + 4], 4
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 0ch], 3
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebp, ecx
        add    ebx, 10h
        inc    dword ptr [esp + 18h]
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_4_04EB
loc_4_0542:
        db  0C7h, 044h, 024h, 024h, 01Eh, 009h, 000h, 000h
        mov    dword ptr [esp + 28h], 0
        db  0C7h, 044h, 024h, 02Ch, 01Eh, 009h, 000h, 000h
        mov    dword ptr [esp + 30h], 0
        sub    esi, esi
        mov    dword ptr [esp + 34h], 0
        db  068h, 090h, 000h, 000h, 000h
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 1ch]
        push   eax
        push   1
        db  068h, 0A8h, 000h, 000h, 000h
        db  0E8h, 016h, 0FBh, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_4_05E0
        db  068h, 094h, 000h, 000h, 000h
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 1ch]
        push   eax
        push   2
        db  068h, 0BCh, 000h, 000h, 000h
        db  0E8h, 0F1h, 0FAh, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_4_05E0
        db  068h, 098h, 000h, 000h, 000h
        push   dword ptr [esp + 1ch]
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 1ch]
        push   eax
        push   3
        db  068h, 0D0h, 000h, 000h, 000h
        db  0E8h, 0CCh, 0FAh, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_4_05E0
        mov    esi, 1
loc_4_05E0:
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0C5h, 0FAh, 0FBh, 0FFh
        add    esp, 8
        push   0
        push   dword ptr [esp + 24h]
        db  0E8h, 0B7h, 0FAh, 0FBh, 0FFh
        add    esp, 8
        or     esi, esi
        jne    SHORT loc_4_060A
        call   sub_4_081E
        jmp    NEAR PTR loc_4_0814
loc_4_060A:
        db  0A1h, 010h, 000h, 000h, 000h
        db  003h, 005h, 08Ch, 000h, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        push   0
        shl    eax, 4
        push   eax
        db  0E8h, 084h, 0FAh, 0FBh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        mov    eax, dword ptr [esp + 1ch]
        push   0
        imul   eax, eax, 66h
        push   eax
        db  0E8h, 06Eh, 0FAh, 0FBh, 0FFh
        mov    dword ptr [esp + 28h], eax
        add    esp, 8
        cmp    dword ptr [esp + 14h], 0
        je     NEAR PTR loc_4_07CE
        or     eax, eax
        je     NEAR PTR loc_4_07B9
        mov    ebp, dword ptr [esp + 3ch]
        mov    ebx, eax
        or     ebp, ebp
        je     SHORT loc_4_06C1
        mov    edx, dword ptr [esp + 14h]
loc_4_0662:
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, dword ptr [esp + 1ch]
        jae    SHORT loc_4_06C1
        mov    dword ptr [edx], ebx
        mov    dword ptr [edx + 4], 4
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edx + 8], eax
        lea    edi, [ebp + 1ch]
        mov    dword ptr [edx + 0ch], 3
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        add    edx, 10h
        inc    dword ptr [esp + 10h]
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    SHORT loc_4_0662
loc_4_06C1:
        db  08Bh, 015h, 0A4h, 000h, 000h, 000h
        or     edx, edx
        je     SHORT loc_4_073A
        mov    ecx, dword ptr [esp + 10h]
        shl    ecx, 4
        mov    ebp, dword ptr [esp + 14h]
        add    ebp, ecx
loc_4_06D8:
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, dword ptr [esp + 1ch]
        jae    SHORT loc_4_073A
        cmp    dword ptr [edx + 0ch], 0
        jne    SHORT loc_4_0734
        mov    dword ptr [ebp], ebx
        mov    dword ptr [ebp + 4], 4
        mov    dword ptr [ebp + 8], edx
        mov    dword ptr [ebp + 0ch], 3
        mov    ecx, 0ffffffffh
        mov    edi, dword ptr [edx + 8]
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        add    ebp, 10h
        inc    dword ptr [esp + 10h]
loc_4_0734:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_4_06D8
loc_4_073A:
        db  068h, 09Ch, 000h, 000h, 000h
        sub    esi, esi
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 1ch]
        push   eax
        push   4
        db  068h, 0E4h, 000h, 000h, 000h
        db  0E8h, 046h, 0F9h, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_4_078B
        db  068h, 0A0h, 000h, 000h, 000h
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 1ch]
        push   eax
        push   5
        db  068h, 0F8h, 000h, 000h, 000h
        db  0E8h, 021h, 0F9h, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_4_078B
        mov    esi, 1
loc_4_078B:
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 01Ah, 0F9h, 0FBh, 0FFh
        add    esp, 8
        push   0
        push   dword ptr [esp + 24h]
        db  0E8h, 00Ch, 0F9h, 0FBh, 0FFh
        add    esp, 8
        or     esi, esi
        jne    SHORT loc_4_07B2
        call   sub_4_081E
        jmp    SHORT loc_4_0814
loc_4_07B2:
        mov    eax, 1
        jmp    SHORT loc_4_0816
loc_4_07B9:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_4_07CE
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0E5h, 0F8h, 0FBh, 0FFh
        add    esp, 8
loc_4_07CE:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_4_07E3
        push   0
        push   dword ptr [esp + 24h]
        db  0E8h, 0D0h, 0F8h, 0FBh, 0FFh
        add    esp, 8
loc_4_07E3:
        call   sub_4_081E
        jmp    SHORT loc_4_0814
loc_4_07EA:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_4_07FF
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0B4h, 0F8h, 0FBh, 0FFh
        add    esp, 8
loc_4_07FF:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_4_0814
        push   0
        push   dword ptr [esp + 24h]
        db  0E8h, 09Fh, 0F8h, 0FBh, 0FFh
        add    esp, 8
loc_4_0814:
        sub    eax, eax
loc_4_0816:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
sub_4_081E:
        db  083h, 03Dh, 090h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0851
        db  0FFh, 035h, 090h, 000h, 000h, 000h
        db  0E8h, 03Eh, 0F8h, 0FBh, 0FFh
        add    esp, 4
        db  068h, 0A8h, 000h, 000h, 000h
        push   80000006h
        db  0E8h, 03Ch, 0F8h, 0FBh, 0FFh
        add    esp, 8
        db  0C7h, 005h, 090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0851:
        db  083h, 03Dh, 094h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0884
        db  0FFh, 035h, 094h, 000h, 000h, 000h
        db  0E8h, 00Bh, 0F8h, 0FBh, 0FFh
        add    esp, 4
        db  068h, 0BCh, 000h, 000h, 000h
        push   80000006h
        db  0E8h, 009h, 0F8h, 0FBh, 0FFh
        add    esp, 8
        db  0C7h, 005h, 094h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0884:
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_08B7
        db  0FFh, 035h, 098h, 000h, 000h, 000h
        db  0E8h, 0D8h, 0F7h, 0FBh, 0FFh
        add    esp, 4
        db  068h, 0D0h, 000h, 000h, 000h
        push   80000006h
        db  0E8h, 0D6h, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0C7h, 005h, 098h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_08B7:
        db  083h, 03Dh, 09Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_4_08EA
        db  0FFh, 035h, 09Ch, 000h, 000h, 000h
        db  0E8h, 0A5h, 0F7h, 0FBh, 0FFh
        add    esp, 4
        db  068h, 0E4h, 000h, 000h, 000h
        push   80000006h
        db  0E8h, 0A3h, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0C7h, 005h, 09Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_08EA:
        db  083h, 03Dh, 0A0h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_091D
        db  0FFh, 035h, 0A0h, 000h, 000h, 000h
        db  0E8h, 072h, 0F7h, 0FBh, 0FFh
        add    esp, 4
        db  068h, 0F8h, 000h, 000h, 000h
        push   80000006h
        db  0E8h, 070h, 0F7h, 0FBh, 0FFh
        add    esp, 8
        db  0C7h, 005h, 0A0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_091D:
        ret
loc_4_091E:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, esi
        cmp    eax, 1
        je     SHORT loc_4_0955
        cmp    eax, 2
        jb     NEAR PTR loc_4_0A64
        cmp    eax, 3
        jbe    NEAR PTR loc_4_09C3
        cmp    eax, 4
        jb     NEAR PTR loc_4_0A64
        cmp    eax, 5
        jbe    NEAR PTR loc_4_0A1A
        jmp    NEAR PTR loc_4_0A64
loc_4_0955:
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ecx], 4
        jae    SHORT loc_4_096F
        mov    eax, 0eah
        jmp    NEAR PTR loc_4_0A64
loc_4_096F:
        mov    dword ptr [ecx], 4
        test   byte ptr [eax + 15h], 1
        jne    SHORT loc_4_098F
        push   eax
        call   sub_4_0062
        mov    edi, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_4_0A62
loc_4_098F:
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [eax + 1ch]
        or     esi, esi
        je     NEAR PTR loc_4_0A62
loc_4_09A4:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     NEAR PTR loc_4_0A62
        push   eax
        call   sub_4_0062
        add    esp, 4
        add    dword ptr [edi], eax
        add    esi, 4
        jne    SHORT loc_4_09A4
        jmp    NEAR PTR loc_4_0A62
loc_4_09C3:
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax + 4]
        test   byte ptr [edi + 15h], 1
        jne    SHORT loc_4_09DC
        push   esi
        push   edi
        call   sub_4_0036
        add    esp, 8
        jmp    SHORT loc_4_09F8
loc_4_09DC:
        mov    ebx, dword ptr [edi + 1ch]
        or     ebx, ebx
        je     SHORT loc_4_09F8
loc_4_09E3:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_4_09F8
        push   esi
        push   eax
        call   sub_4_0036
        add    esp, 8
        add    ebx, 4
        jne    SHORT loc_4_09E3
loc_4_09F8:
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 18h]
        or     eax, eax
        je     SHORT loc_4_0A62
        push   eax
        db  0B8h, 06Bh, 004h, 000h, 000h
        cmp    esi, 2
        je     SHORT loc_4_0A12
        db  0B8h, 073h, 004h, 000h, 000h
loc_4_0A12:
        push   eax
        db  0E8h, 0A0h, 0F6h, 0FBh, 0FFh
        jmp    SHORT loc_4_0A62
loc_4_0A1A:
        mov    ecx, dword ptr [esp + 20h]
        cmp    dword ptr [ecx], 4
        jae    SHORT loc_4_0A2A
        mov    eax, 0eah
        jmp    SHORT loc_4_0A64
loc_4_0A2A:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx], 4
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_4_0A3D
        jmp    SHORT loc_4_0A58
loc_4_0A3D:
        cmp    esi, 4
        push   eax
        jne    SHORT loc_4_0A4A
        db  068h, 06Bh, 004h, 000h, 000h
        jmp    SHORT loc_4_0A4F
loc_4_0A4A:
        db  068h, 073h, 004h, 000h, 000h
loc_4_0A4F:
        db  0E8h, 064h, 0F6h, 0FBh, 0FFh
        or     eax, eax
        je     SHORT loc_4_0A62
loc_4_0A58:
        mov    edi, dword ptr [esp + 1ch]
        mov    dword ptr [edi], 1
loc_4_0A62:
        sub    eax, eax
loc_4_0A64:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_0A68:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   1
        inc    dword ptr [esi + 0ch]
        lea    eax, [esp + 8]
        push   dword ptr [esi + 4]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        db  0E8h, 0A9h, 0F5h, 0FBh, 0FFh
        or     eax, eax
        jne    SHORT loc_4_0A99
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 9]
        mov    word ptr [esi + 10h], ax
        jmp    SHORT loc_4_0A9C
loc_4_0A99:
        dec    dword ptr [esi + 0ch]
loc_4_0A9C:
        pop    esi
        add    esp, 8
        ret
sub_4_0AA1:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        je     SHORT loc_4_0AD7
        mov    ax, word ptr [esi + 10h]
        or     ax, ax
        je     SHORT loc_4_0AD7
        dec    ecx
        mov    dword ptr [esi + 0ch], ecx
        push   0
        push   eax
        db  0E8h, 092h, 0F5h, 0FBh, 0FFh
        or     eax, eax
        jne    SHORT loc_4_0AD4
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_4_0AD7
        mov    word ptr [esi + 10h], 0
        jmp    SHORT loc_4_0AD7
loc_4_0AD4:
        inc    dword ptr [esi + 0ch]
loc_4_0AD7:
        pop    esi
        ret
sub_4_0AD9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        je     NEAR PTR loc_4_0B76
        mov    ebx, dword ptr [esp + 18h]
        or     ebx, ebx
        mov    edx, dword ptr [esp + 1ch]
        je     NEAR PTR loc_4_0B7A
        or     edx, edx
        je     SHORT loc_4_0B7A
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 040h, 001h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebp, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        jmp    SHORT loc_4_0B99
loc_4_0B76:
        mov    edx, dword ptr [esp + 1ch]
loc_4_0B7A:
        db  0BFh, 044h, 001h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
loc_4_0B99:
        and    ecx, 3
        rep movsb
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_0BA3:
        sub    esp, 218h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        lea    eax, [esp + 1ch]
        push   eax
        db  068h, 00Ch, 001h, 000h, 000h
        push   80000002h
        db  0E8h, 0A5h, 0F4h, 0FBh, 0FFh
        add    esp, 0ch
        cmp    eax, edi
        jne    NEAR PTR loc_4_0D11
        lea    eax, [esp + 20h]
        push   0ffh
        push   eax
        push   edi
        push   dword ptr [esp + 28h]
        db  0E8h, 0A6h, 0F4h, 0FBh, 0FFh
        add    esp, 10h
        cmp    eax, edi
        jne    NEAR PTR loc_4_0D05
        mov    esi, dword ptr [esp + 22ch]
        sub    ebx, ebx
        mov    ebp, 105h
loc_4_0BFB:
        mov    dword ptr [esp + 14h], ebx
        mov    dword ptr [esp + 10h], ebx
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   esi
        db  0E8h, 065h, 0F4h, 0FBh, 0FFh
        add    esp, 0ch
        cmp    eax, ebx
        jne    NEAR PTR loc_4_0CC1
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 24h]
        db  0E8h, 037h, 0F4h, 0FBh, 0FFh
        add    esp, 0ch
        cmp    eax, ebx
        jne    NEAR PTR loc_4_0CC1
        mov    dword ptr [esp + 18h], ebp
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 124h]
        push   eax
        push   ebx
        push   ebx
        db  068h, 050h, 000h, 000h, 000h
        push   dword ptr [esp + 24h]
        db  0E8h, 033h, 0F4h, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, ebx
        jne    SHORT loc_4_0C84
        lea    eax, [esp + 120h]
        push   dword ptr [esp + 18h]
        push   eax
        push   1
        push   ebx
        db  068h, 050h, 000h, 000h, 000h
        push   dword ptr [esp + 28h]
        db  0E8h, 017h, 0F4h, 0FBh, 0FFh
        add    esp, 18h
loc_4_0C84:
        mov    dword ptr [esp + 18h], ebp
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 124h]
        push   eax
        push   ebx
        push   ebx
        db  068h, 055h, 000h, 000h, 000h
        push   dword ptr [esp + 24h]
        db  0E8h, 0EBh, 0F3h, 0FBh, 0FFh
        add    esp, 18h
        cmp    eax, ebx
        jne    SHORT loc_4_0CC1
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 124h]
        push   eax
        call   sub_4_0D1C
        add    esp, 8
loc_4_0CC1:
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_4_0CD3
        push   dword ptr [esp + 10h]
        db  0E8h, 0A0h, 0F3h, 0FBh, 0FFh
        add    esp, 4
loc_4_0CD3:
        cmp    dword ptr [esp + 14h], ebx
        je     SHORT loc_4_0CE5
        push   dword ptr [esp + 14h]
        db  0E8h, 08Eh, 0F3h, 0FBh, 0FFh
        add    esp, 4
loc_4_0CE5:
        inc    edi
        lea    eax, [esp + 20h]
        push   0ffh
        push   eax
        push   edi
        push   dword ptr [esp + 28h]
        db  0E8h, 08Eh, 0F3h, 0FBh, 0FFh
        add    esp, 10h
        cmp    eax, ebx
        je     NEAR PTR loc_4_0BFB
loc_4_0D05:
        push   dword ptr [esp + 1ch]
        db  0E8h, 062h, 0F3h, 0FBh, 0FFh
        add    esp, 4
loc_4_0D11:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 218h
        ret
sub_4_0D1C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    ebx, dword ptr [esp + 14h]
        lea    edx, [ecx - 1]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx + 13h]
        push   0
        push   eax
        db  0E8h, 059h, 0F3h, 0FBh, 0FFh
        add    esp, 8
        mov    edx, eax
        or     edx, edx
        mov    eax, 0
        je     NEAR PTR loc_4_0DE1
        db  0A1h, 0A4h, 000h, 000h, 000h
        mov    dword ptr [edx], eax
        db  089h, 015h, 0A4h, 000h, 000h, 000h
        lea    ebp, [edx + 12h]
        mov    dword ptr [edx + 4], ebp
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebp, ecx
        mov    dword ptr [edx + 8], ebp
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [edx + 0ch], 0
        db  0A1h, 08Ch, 000h, 000h, 000h
        inc    eax
        db  0A3h, 08Ch, 000h, 000h, 000h
        mov    eax, 1
loc_4_0DE1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_0DE6:
        push   ebx
        push   esi
        db  0A1h, 0A4h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_0E29
        mov    esi, dword ptr [esp + 0ch]
loc_4_0DF5:
        mov    ecx, esi
        mov    edx, dword ptr [eax + 8]
loc_4_0DFA:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edx]
        jne    SHORT loc_4_0E1A
        or     bl, bl
        je     SHORT loc_4_0E16
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edx + 1]
        jne    SHORT loc_4_0E1A
        add    ecx, 2
        add    edx, 2
        or     bl, bl
        jne    SHORT loc_4_0DFA
loc_4_0E16:
        sub    ecx, ecx
        jmp    SHORT loc_4_0E1F
loc_4_0E1A:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_4_0E1F:
        or     ecx, ecx
        je     SHORT loc_4_0E2B
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_4_0DF5
loc_4_0E29:
        sub    eax, eax
loc_4_0E2B:
        pop    esi
        pop    ebx
        ret
sub_4_0E2E:
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        mov    eax, 0
        je     SHORT loc_4_0E49
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 3e8h
        sub    edx, edx
        div    ecx
loc_4_0E49:
        ret
sub_4_0E4A:
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        or     ecx, ecx
        je     SHORT loc_4_0E6D
        mov    eax, dword ptr [esp + 8]
        cmp    ecx, eax
        jb     SHORT loc_4_0E6D
        mov    esi, 64h
        imul   eax, esi
        sub    edx, edx
        div    ecx
        sub    esi, eax
        mov    eax, esi
        jmp    SHORT loc_4_0E72
loc_4_0E6D:
        mov    eax, 64h
loc_4_0E72:
        pop    esi
        ret
loc_4_0E74:
        push   ebp
        mov    ebp, esp
        db  0A1h, 04Ch, 001h, 000h, 000h
        cmp    dword ptr [ebp + 0ch], eax
        jne    SHORT loc_4_0EB4
        cmp    dword ptr [ebp + 8], 11h
        jne    SHORT loc_4_0EA7
        db  083h, 03Dh, 050h, 001h, 000h, 000h, 000h
        jne    SHORT loc_4_0E9F
        db  0C7h, 005h, 05Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_4_0F5B
loc_4_0E9F:
        db  0FFh, 005h, 050h, 001h, 000h, 000h
        jmp    SHORT loc_4_0EB4
loc_4_0EA7:
        db  0FFh, 00Dh, 050h, 001h, 000h, 000h
        jne    SHORT loc_4_0EB4
        call   sub_4_0F72
loc_4_0EB4:
        leave
        ret
        db  056h, 057h, 0A3h, 048h, 001h, 000h, 000h, 0A1h, 048h, 001h, 000h, 000h, 083h, 0F8h, 000h, 074h
        db  00Ah, 050h, 0CDh, 020h, 002h, 000h, 048h, 000h, 083h, 0C4h, 004h, 05Fh, 05Eh, 0C3h
loc_4_0ED4:
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 00Ah, 001h, 001h, 000h
loc_4_0EDE:
        inc    eax
        db  0CDh, 020h, 013h, 001h, 001h, 000h
        db  0CDh, 020h, 00Bh, 001h, 001h, 000h
        jne    SHORT loc_4_0EDE
        pop    edi
        pop    esi
        ret
loc_4_0EF0:
        push   esi
        push   edi
        xor    eax, eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
loc_4_0EFA:
        inc    eax
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_4_0EFA
        pop    edi
        pop    esi
        ret
loc_4_0F0C:
        push   esi
        push   edi
        push   ebx
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    ecx, eax
        db  08Bh, 01Dh, 05Ch, 001h, 000h, 000h
        db  02Bh, 005h, 058h, 001h, 000h, 000h
        db  089h, 00Dh, 058h, 001h, 000h, 000h
        db  0C7h, 005h, 05Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        push   eax
        push   ebx
        call   sub_4_0E2E
        pop    ecx
        pop    ecx
        db  03Bh, 005h, 054h, 001h, 000h, 000h
        jle    SHORT loc_4_0F49
        db  0A3h, 054h, 001h, 000h, 000h
loc_4_0F49:
        db  0FFh, 035h, 054h, 001h, 000h, 000h
        push   eax
        call   sub_4_0E4A
        pop    ecx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        ret
sub_4_0F5B:
        db  0BEh, 07Eh, 00Fh, 000h, 000h
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  0A3h, 058h, 001h, 000h, 000h
        ret
sub_4_0F72:
        db  0BEh, 07Eh, 00Fh, 000h, 000h
        db  0CDh, 020h, 02Bh, 001h, 001h, 000h
        ret
loc_4_0F7E:
        db  0FFh, 005h, 05Ch, 001h, 000h, 000h
        clc
        ret
        db  000h, 000h
_PTEXT ends
        end
