; sbawe_pcode_03.asm -- SBAWE object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  083h, 0ECh, 018h, 033h, 0C9h, 053h, 056h, 08Bh, 074h, 024h, 024h, 057h, 055h, 08Dh, 056h, 024h
        db  066h, 08Bh, 03Ah, 033h, 0C0h, 08Dh, 01Ch, 007h, 08Dh, 02Ch, 001h, 066h, 089h, 05Ch, 06Ch, 010h
        db  040h, 083h, 0F8h, 004h, 07Ch, 0EFh, 083h, 0C2h, 002h, 083h, 0C1h, 004h, 083h, 0F9h, 00Ch, 07Ch
        db  0DFh, 056h, 08Dh, 046h, 00Ch, 08Dh, 04Ch, 024h, 014h, 050h, 068h, 000h, 000h, 000h, 000h, 051h
        db  06Ah, 00Ch, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 014h, 08Bh, 0F8h, 085h, 0FFh, 075h, 009h
        db  056h, 0E8h, 000h, 000h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 0C7h, 05Dh, 05Fh, 05Eh, 05Bh, 083h
        db  0C4h, 018h, 0C3h
sub_3_0063:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        push   ebp
        xor    edi, edi
        cmp    esi, edi
        je     SHORT loc_3_0088
        mov    eax, dword ptr [esp + 2ch]
loc_3_007A:
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_3_0092
        mov    edi, esi
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_3_007A
loc_3_0088:
        xor    eax, eax
loc_3_008A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_3_0092:
        lea    ecx, [esi + 24h]
        xor    eax, eax
loc_3_0097:
        xor    edx, edx
loc_3_0099:
        mov    bx, word ptr [ecx]
        lea    ebp, [edx + eax]
        add    bx, dx
        inc    edx
        mov    word ptr [esp + ebp*2 + 10h], bx
        cmp    edx, 4
        jl     SHORT loc_3_0099
        add    ecx, 2
        add    eax, 4
        cmp    eax, 0ch
        jl     SHORT loc_3_0097
        lea    eax, [esp + 10h]
        push   eax
        push   0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00C4:
        add    esp, 8
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00CD:
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        test   edi, edi
        je     SHORT loc_3_00DC
        mov    dword ptr [edi + 4], eax
        jmp    SHORT loc_3_00E1
loc_3_00DC:
        db  0A3h, 000h, 000h, 000h, 000h
loc_3_00E1:
        mov    eax, esi
        jmp    SHORT loc_3_008A
sub_3_00E5:
        push   dword ptr [esp + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00EE:
        add    esp, 4
        ret
sub_3_00F2:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_3_010C
        mov    eax, dword ptr [esp + 4]
loc_3_0100:
        cmp    dword ptr [ecx + 8], eax
        je     SHORT loc_3_0112
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_3_0100
loc_3_010C:
        mov    eax, 1
loc_3_0111:
        ret
loc_3_0112:
        cmp    dword ptr [ecx], 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_3_0111
        db  0CCh, 0CCh
loc_3_011C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, esp
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0dh
        je     SHORT loc_3_016E
        cmp    eax, 0
        jne    SHORT loc_3_0180
        push   ecx
        mov    ecx, dword ptr [esp + 4]
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
loc_3_0147:
        cmp    edi, 0
        je     SHORT loc_3_016C
        cmp    dword ptr [edi + 8], ecx
        je     SHORT loc_3_0156
        mov    edi, dword ptr [edi + 4]
        jmp    SHORT loc_3_0147
loc_3_0156:
        push   3
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        pop    edi
        pop    ecx
        jmp    SHORT loc_3_0180
loc_3_016C:
        pop    edi
        pop    ecx
loc_3_016E:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
loc_3_0180:
        add    esp, 4
        pop    ebp
        stc
        ret
sub_3_0186:
        sub    esp, 4
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_3_01D9
loc_3_0194:
        mov    ecx, dword ptr [esi + 8]
        xor    eax, eax
loc_3_0199:
        mov    edx, ecx
        inc    eax
        shr    edx, 1ch
        shl    ecx, 4
        db  08Ah, 092h, 000h, 000h, 000h, 000h
        db  088h, 090h, 013h, 000h, 000h, 000h
        cmp    eax, 8
        jl     SHORT loc_3_0199
        lea    eax, [esp + 4]
        push   eax
        push   8
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01CF:
        add    esp, 18h
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_3_0194
loc_3_01D9:
        pop    esi
        add    esp, 4
        ret
loc_3_01DE:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax]
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   edi
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 10h], 0
        mov    dword ptr [eax], 0
        je     NEAR PTR loc_3_02E5
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 14h]
loc_3_0210:
        mov    eax, dword ptr [ebx + 4]
        shl    eax, 4
        db  08Bh, 080h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebx], eax
        cmp    dword ptr [ebp - 8], eax
        jb     NEAR PTR loc_3_02CB
        mov    eax, dword ptr [ebx + 4]
        cmp    eax, 7
        ja     SHORT loc_3_0236
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_3_0236:
        mov    dword ptr [edi], 0
        jmp    SHORT loc_3_02B8
loc_3_023E:
        mov    eax, dword ptr [esi + 1558h]
        jmp    SHORT loc_3_02B6
loc_3_0246:
        cmp    dword ptr [esi + 1558h], 10h
        jne    SHORT loc_3_0258
        movzx  eax, byte ptr [esi + 155ch]
        jmp    SHORT loc_3_02B6
loc_3_0258:
        mov    dword ptr [edi], 0ffffffffh
        jmp    SHORT loc_3_02B8
loc_3_0260:
        cmp    dword ptr [esi + 1558h], 10h
        jne    SHORT loc_3_0272
        movzx  eax, byte ptr [esi + 155dh]
        jmp    SHORT loc_3_02B6
loc_3_0272:
        mov    dword ptr [edi], 0ffffffffh
        jmp    SHORT loc_3_02B8
loc_3_027A:
        movzx  eax, word ptr [esi + 24h]
        movzx  edx, word ptr [esi + 26h]
        mov    ecx, eax
        sub    ecx, edx
        cmp    ecx, -4
        jne    SHORT loc_3_02B6
        xor    eax, eax
        mov    ax, word ptr [esi + 24h]
        and    eax, 0fff8h
        jmp    SHORT loc_3_02B6
loc_3_0298:
        mov    eax, dword ptr [esi + 154eh]
        jmp    SHORT loc_3_02B6
loc_3_02A0:
        mov    eax, dword ptr [esi + 2ch]
        jmp    SHORT loc_3_02B6
loc_3_02A5:
        mov    eax, dword ptr [esi + 30h]
        jmp    SHORT loc_3_02B6
loc_3_02AA:
        lea    eax, [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_02B3:
        add    esp, 4
loc_3_02B6:
        mov    dword ptr [edi], eax
loc_3_02B8:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 18h]
        add    dword ptr [eax], ecx
        mov    dword ptr [ebx + 8], edi
        mov    eax, dword ptr [ebx]
        add    edi, eax
        sub    dword ptr [ebp - 8], eax
        jmp    SHORT loc_3_02D9
loc_3_02CB:
        mov    dword ptr [ebx + 8], 0
        mov    dword ptr [ebp - 4], 0eah
loc_3_02D9:
        add    ebx, 0ch
        dec    dword ptr [ebp + 10h]
        jne    NEAR PTR loc_3_0210
loc_3_02E5:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_030F:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        xor    edi, edi
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_3_03A4
        xor    edx, edx
        cmp    byte ptr [esi + edi], dl
        je     SHORT loc_3_0335
loc_3_0328:
        cmp    byte ptr [esi + edi], 20h
        jne    SHORT loc_3_0335
        inc    edi
        cmp    byte ptr [esi + edi], 0
        jne    SHORT loc_3_0328
loc_3_0335:
        mov    cl, byte ptr [esi + edi]
        cmp    byte ptr [esp + 10h], cl
        je     SHORT loc_3_0360
        movsx  eax, cl
        movsx  ebx, byte ptr [esp + 10h]
        sub    eax, ebx
        cmp    eax, 20h
        je     SHORT loc_3_0360
        test   cl, cl
        je     SHORT loc_3_039F
loc_3_0351:
        cmp    byte ptr [esi + edi], 20h
        je     SHORT loc_3_039F
        inc    edi
        cmp    byte ptr [esi + edi], 0
        jne    SHORT loc_3_0351
        jmp    SHORT loc_3_039F
loc_3_0360:
        mov    ebx, edi
        mov    eax, edi
        test   cl, cl
        je     SHORT loc_3_0375
loc_3_0368:
        cmp    byte ptr [esi + eax], 20h
        je     SHORT loc_3_0375
        inc    eax
        cmp    byte ptr [esi + eax], 0
        jne    SHORT loc_3_0368
loc_3_0375:
        cmp    byte ptr [esi + eax], dl
        je     SHORT loc_3_039A
loc_3_037A:
        cmp    byte ptr [esi + eax], 20h
        jne    SHORT loc_3_0387
        inc    eax
        cmp    byte ptr [esi + eax], 0
        jne    SHORT loc_3_037A
loc_3_0387:
        cmp    byte ptr [esi + eax], dl
        je     SHORT loc_3_039A
loc_3_038C:
        mov    cl, byte ptr [esi + eax]
        inc    eax
        mov    byte ptr [esi + edi], cl
        inc    edi
        cmp    byte ptr [esi + eax], 0
        jne    SHORT loc_3_038C
loc_3_039A:
        mov    byte ptr [esi + edi], dl
        mov    edi, ebx
loc_3_039F:
        cmp    byte ptr [esi + edi], dl
        jne    SHORT loc_3_0328
loc_3_03A4:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_03A8:
        push   ebp
        mov    ebp, esp
        sub    esp, 64h
        push   esi
        lea    eax, [ebp - 64h]
        mov    dword ptr [ebp - 14h], 50h
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 04Ah, 004h
        add    esp, 0ch
        cmp    byte ptr [ebp - 64h], 0
        je     SHORT loc_3_03F0
        lea    eax, [ebp - 64h]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_030F
        add    esp, 8
loc_3_03F0:
        xor    esi, esi
        cmp    byte ptr [ebp - 64h], 0
        je     SHORT loc_3_0400
loc_3_03F8:
        inc    esi
        cmp    byte ptr [ebp + esi - 64h], 0
        jne    SHORT loc_3_03F8
loc_3_0400:
        test   esi, esi
        je     SHORT loc_3_0416
        cmp    byte ptr [ebp + esi - 65h], 20h
        jne    SHORT loc_3_040C
        dec    esi
loc_3_040C:
        test   esi, esi
        je     SHORT loc_3_0416
        mov    byte ptr [ebp + esi - 64h], 20h
        inc    esi
loc_3_0416:
        mov    al, byte ptr [ebp + 8]
        inc    esi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ebp + esi - 65h], al
        xor    eax, eax
loc_3_0423:
        test   ecx, 0f0000000h
        jne    SHORT loc_3_0434
        shl    ecx, 4
        inc    eax
        cmp    eax, 8
        jl     SHORT loc_3_0423
loc_3_0434:
        cmp    eax, 8
        jge    SHORT loc_3_0456
        mov    edx, 8
        sub    edx, eax
loc_3_0440:
        mov    eax, ecx
        inc    esi
        shr    eax, 1ch
        shl    ecx, 4
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        dec    edx
        mov    byte ptr [ebp + esi - 65h], al
        jne    SHORT loc_3_0440
loc_3_0456:
        mov    byte ptr [ebp + esi - 64h], 0
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 64h]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0482:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 64h
        test   ecx, ecx
        je     NEAR PTR loc_3_052C
        mov    eax, dword ptr [ebp + 8]
loc_3_0499:
        cmp    dword ptr [ecx + 8], eax
        je     SHORT loc_3_04AA
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_3_0499
        jmp    NEAR PTR loc_3_052C
loc_3_04AA:
        mov    eax, dword ptr [ecx + 4]
        test   eax, eax
        je     SHORT loc_3_04CA
        xor    ecx, ecx
        mov    cx, word ptr [eax + 24h]
        and    ecx, 0fff8h
        push   ecx
        push   45h
        call   sub_3_03A8
        add    esp, 8
        jmp    SHORT loc_3_052C
loc_3_04CA:
        mov    dword ptr [ebp - 14h], 50h
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 64h]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 04Ah, 004h
        add    esp, 0ch
        cmp    byte ptr [ebp - 64h], 0
        je     SHORT loc_3_052C
        lea    eax, [ebp - 64h]
        push   eax
        push   45h
        call   sub_3_030F
        add    esp, 8
        lea    eax, [ebp - 64h]
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_3_052C:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0530:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 14h]
        mov    dword ptr [ebp - 4], 10h
        mov    byte ptr [ebp - 14h], 0
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_055C:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_0592
        cmp    dword ptr [ebp - 4], 4
        jb     SHORT loc_3_0592
        cmp    byte ptr [ebp - 14h], 73h
        je     SHORT loc_3_0575
        cmp    byte ptr [ebp - 14h], 53h
        jne    SHORT loc_3_0592
loc_3_0575:
        cmp    byte ptr [ebp - 13h], 62h
        je     SHORT loc_3_0581
        cmp    byte ptr [ebp - 13h], 42h
        jne    SHORT loc_3_0592
loc_3_0581:
        cmp    byte ptr [ebp - 12h], 31h
        jne    SHORT loc_3_0592
        cmp    byte ptr [ebp - 11h], 36h
        mov    eax, 1
        je     SHORT loc_3_0594
loc_3_0592:
        xor    eax, eax
loc_3_0594:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0598:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        lea    eax, [ebp - 10h]
        push   esi
        lea    ecx, [ebp - 20h]
        push   edi
        lea    edx, [ebp - 0ch]
        push   0
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 20h], 4
        push   eax
        push   ecx
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_05BF:
        add    esp, 14h
        cmp    dword ptr [ebp - 0ch], 10h
        je     SHORT loc_3_05E1
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_3_05E1
        cmp    dword ptr [ebp - 0ch], 1
        je     SHORT loc_3_05E1
        cmp    dword ptr [ebp - 0ch], 2
        je     SHORT loc_3_05E1
        xor    eax, eax
        jmp    NEAR PTR loc_3_0806
loc_3_05E1:
        cmp    dword ptr [ebp - 0ch], 10h
        mov    si, 0ffffh
        jne    NEAR PTR loc_3_06B6
        cmp    byte ptr [ebp - 0fh], 0
        jne    SHORT loc_3_05FC
        xor    eax, eax
        jmp    NEAR PTR loc_3_0806
loc_3_05FC:
        mov    bl, byte ptr [ebp - 10h]
        push   0
        push   edi
        lea    eax, [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_060B:
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        push   0
        push   dword ptr [ebp - 1ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_061C:
        add    esp, 0ch
loc_3_061F:
        cmp    dword ptr [ebp - 8], edi
        je     SHORT loc_3_069B
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        lea    ecx, [ebp - 20h]
        push   ecx
        lea    edx, [ebp - 0ch]
        mov    byte ptr [ebp - 10h], 0ffh
        mov    dword ptr [ebp - 20h], 4
        push   edx
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0645:
        add    esp, 14h
        cmp    byte ptr [ebp - 10h], bl
        jne    SHORT loc_3_069B
        push   dword ptr [ebp - 8]
        call   sub_3_0530
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_069B
        push   2
        lea    ecx, [ebp - 18h]
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_066A:
        add    esp, 0ch
        lea    edx, [ebp - 14h]
        push   0
        push   0
        push   2
        push   dword ptr [ebp - 18h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_067F:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 14h]
        mov    ax, word ptr [ecx + 4]
        mov    cx, ax
        and    cx, 0ff0fh
        cmp    cx, 200h
        jne    SHORT loc_3_069B
        mov    si, ax
loc_3_069B:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06A9:
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_061F
        jmp    SHORT loc_3_0703
loc_3_06B6:
        xor    ebx, ebx
        lea    eax, [ebp - 1ch]
        push   ebx
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06C3:
        add    esp, 0ch
        lea    ecx, [ebp - 18h]
        push   2
        push   dword ptr [ebp - 1ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06D4:
        add    esp, 0ch
        lea    edx, [ebp - 14h]
        push   ebx
        push   ebx
        push   2
        push   dword ptr [ebp - 18h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06E7:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 14h]
        mov    ax, word ptr [ecx + 4]
        mov    cx, ax
        and    cx, 0ff0fh
        cmp    cx, 200h
        jne    SHORT loc_3_0703
        mov    si, ax
loc_3_0703:
        cmp    si, 0ffffh
        jne    SHORT loc_3_0711
        xor    eax, eax
        jmp    NEAR PTR loc_3_0806
loc_3_0711:
        lea    ebx, [esi + 4]
        xor    edi, edi
        add    si, 5
        mov    al, 0feh
        mov    dx, bx
        mov    dword ptr [ebp - 4], edi
        out    dx, al
        cmp    dword ptr [ebp - 4], 0c8h
        jae    SHORT loc_3_0738
loc_3_072C:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0c8h
        jb     SHORT loc_3_072C
loc_3_0738:
        mov    dx, si
        in     al, dx
        cmp    al, 0ffh
        mov    cl, al
        je     SHORT loc_3_0785
        mov    dx, bx
        mov    al, 0fdh
        out    dx, al
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 4], 0c8h
        jae    SHORT loc_3_0764
loc_3_0758:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0c8h
        jb     SHORT loc_3_0758
loc_3_0764:
        mov    dx, si
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_3_0785
        cmp    cl, 10h
        jb     SHORT loc_3_0776
        cmp    cl, 1fh
        jbe    SHORT loc_3_0780
loc_3_0776:
        cmp    cl, 30h
        jb     SHORT loc_3_0785
        cmp    cl, 3fh
        ja     SHORT loc_3_0785
loc_3_0780:
        mov    edi, 2
loc_3_0785:
        xor    ecx, ecx
        test   edi, edi
        jne    SHORT loc_3_0804
        mov    dx, bx
        mov    al, 0ffh
        out    dx, al
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebp - 4], 0c8h
        jae    SHORT loc_3_07A9
loc_3_079D:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0c8h
        jb     SHORT loc_3_079D
loc_3_07A9:
        mov    dx, si
        mov    al, 4
        out    dx, al
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 4], 0c8h
        jae    SHORT loc_3_07CB
loc_3_07BF:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0c8h
        jb     SHORT loc_3_07BF
loc_3_07CB:
        mov    dx, si
        in     al, dx
        cmp    al, 0ffh
        jne    SHORT loc_3_0804
        mov    dx, bx
        mov    al, 82h
        out    dx, al
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp - 4], 0c8h
        jae    SHORT loc_3_07F5
loc_3_07E9:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0c8h
        jb     SHORT loc_3_07E9
loc_3_07F5:
        mov    dx, si
        in     al, dx
        and    al, 0f0h
        cmp    al, 80h
        jne    SHORT loc_3_0804
        mov    edi, 1
loc_3_0804:
        mov    eax, edi
loc_3_0806:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_080D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        mov    eax, 1
        ja     SHORT loc_3_0863
        xor    esi, esi
        lea    eax, [ebp - 8]
        push   esi
        push   2
        push   esi
        push   4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_083C:
        add    esp, 14h
        lea    ecx, [ebp - 0ch]
        push   esi
        push   dword ptr [ebp - 8]
        push   0fcffh
        push   0fc00h
        push   10h
        push   esi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0859:
        add    esp, 1ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_3_0863:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0868:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        lea    eax, [ebp - 14h]
        push   esi
        lea    ecx, [ebp - 18h]
        push   edi
        xor    esi, esi
        push   esi
        lea    edx, [ebp - 20h]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], 4
        push   ecx
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0890:
        add    esp, 14h
        cmp    dword ptr [ebp - 20h], 10h
        jne    NEAR PTR loc_3_0A10
        mov    al, byte ptr [ebp - 14h]
        push   1
        push   edi
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    byte ptr [ebp - 1], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08AF:
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        push   esi
        push   esi
        push   2
        push   dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08C2:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 8]
        cmp    word ptr [ecx], 1
        ja     NEAR PTR loc_3_0A10
        cmp    word ptr [ecx + 0eh], 4
        jne    NEAR PTR loc_3_0A10
        push   esi
        lea    eax, [ebp - 44h]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08E8:
        add    esp, 0ch
        lea    ecx, [ebp - 10h]
        push   esi
        push   dword ptr [ebp - 44h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08F8:
        add    esp, 0ch
loc_3_08FB:
        cmp    dword ptr [ebp - 10h], edi
        je     NEAR PTR loc_3_0985
        push   esi
        lea    eax, [ebp - 14h]
        push   eax
        lea    ecx, [ebp - 18h]
        push   ecx
        lea    edx, [ebp - 20h]
        mov    byte ptr [ebp - 14h], 0ffh
        mov    dword ptr [ebp - 18h], 4
        push   edx
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0924:
        add    esp, 14h
        mov    cl, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 14h], cl
        jne    SHORT loc_3_0985
        push   esi
        lea    ecx, [ebp - 48h]
        push   dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 40h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0940:
        add    esp, 10h
        test   byte ptr [ebp - 40h], 8
        je     SHORT loc_3_0985
        push   2
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0957:
        add    esp, 0ch
        lea    edx, [ebp - 8]
        push   esi
        push   esi
        push   2
        push   dword ptr [ebp - 0ch]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_096A:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 8]
        mov    bx, word ptr [ecx + 4]
        mov    ax, bx
        and    ax, 0ff0fh
        cmp    ax, 200h
        je     NEAR PTR loc_3_0A17
loc_3_0985:
        push   esi
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0992:
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_08FB
        push   1
        lea    eax, [ebp - 0ch]
        push   edi
        xor    ebx, ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09AB:
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        push   ebx
        push   ebx
        push   2
        lea    edi, [ebp - 38h]
        push   dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09C1:
        add    esp, 14h
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, 6
        rep movsd
        mov    esi, 400h
        push   ebx
        add    word ptr [ebp - 28h], si
        push   18h
        add    word ptr [ebp - 26h], si
        lea    ecx, [ebp - 38h]
        push   ecx
        lea    edx, [ebp - 8]
        push   2
        push   dword ptr [ebp - 0ch]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_09F0:
        add    word ptr [ebp - 28h], si
        add    esp, 18h
        add    word ptr [ebp - 26h], si
        lea    ecx, [ebp - 38h]
        push   ebx
        push   18h
        push   ecx
        push   2
        push   dword ptr [ebp - 0ch]
        push   ebx
loc_3_0A08:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A0D:
        add    esp, 18h
loc_3_0A10:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0A17:
        add    bx, 400h
        push   0
        push   2
        lea    eax, [ebp - 1ch]
        movzx  esi, bx
        push   0
        push   4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A30:
        add    esp, 14h
        lea    ecx, [esi + 3]
        lea    eax, [ebp - 3ch]
        push   0
        push   dword ptr [ebp - 1ch]
        push   ecx
        push   esi
        push   4
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A4A:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_0A10
        lea    ecx, [esi + 403h]
        push   0
        add    esi, 400h
        lea    eax, [ebp - 3ch]
        push   dword ptr [ebp - 1ch]
        push   ecx
        push   esi
        push   4
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A71:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_0A10
        push   1
        lea    eax, [ebp - 0ch]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A84:
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        push   0
        push   0
        push   2
        push   dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A99:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 10h], bx
        lea    edx, [ebx + 3]
        mov    ecx, dword ptr [ebp - 8]
        lea    edi, [ebp - 38h]
        mov    word ptr [ecx + 12h], dx
        mov    ecx, 6
        mov    esi, dword ptr [ebp - 8]
        lea    edx, [ebx + 403h]
        rep movsd
        mov    word ptr [ebp - 26h], dx
        lea    eax, [ebx + 400h]
        mov    word ptr [ebp - 28h], ax
        push   0
        push   18h
        lea    ecx, [ebp - 38h]
        push   ecx
        lea    edx, [ebp - 8]
        push   2
        push   dword ptr [ebp - 0ch]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AE4:
        add    esp, 18h
        lea    ecx, [ebx + 800h]
        add    bx, 803h
        lea    eax, [ebp - 38h]
        mov    word ptr [ebp - 28h], cx
        push   0
        mov    word ptr [ebp - 26h], bx
        push   18h
        push   eax
        push   2
        push   dword ptr [ebp - 0ch]
        push   0
        jmp    NEAR PTR loc_3_0A08
sub_3_0B0E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0f8h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        push   1
        push   0f0h
        lea    eax, [esi + 15e1h]
        push   eax
        mov    dword ptr [esi + 8], edi
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B39:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0EA8
        push   0
        lea    eax, [ebp - 0f8h]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B53:
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_3_0B6C
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B62:
        add    esp, 4
        mov    eax, ebx
        jmp    NEAR PTR loc_3_0EAD
loc_3_0B6C:
        push   0
        lea    eax, [esi + 1560h]
        push   80h
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B80:
        add    esp, 10h
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_3_0B99
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B8F:
        add    esp, 4
        mov    eax, ebx
        jmp    NEAR PTR loc_3_0EAD
loc_3_0B99:
        lea    ebx, [esi + 1558h]
        push   0
        lea    eax, [esi + 155ch]
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 0ch], 4
        push   eax
        push   ecx
        push   ebx
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0BBC:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0EA8
        cmp    dword ptr [ebx], 10h
        jne    NEAR PTR loc_3_0D46
        cmp    word ptr [ebp - 9ch], 1
        jne    NEAR PTR loc_3_0D46
        cmp    word ptr [ebp - 72h], 4
        jne    NEAR PTR loc_3_0D46
        push   0
        lea    eax, [ebp - 1ch]
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0BF5:
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        push   0
        push   dword ptr [ebp - 1ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0C06:
        add    esp, 0ch
loc_3_0C09:
        cmp    dword ptr [ebp - 8], edi
        je     NEAR PTR loc_3_0CC4
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 20h]
        mov    byte ptr [ebp - 18h], 0ffh
        mov    dword ptr [ebp - 0ch], 4
        push   edx
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0C33:
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0CC4
        cmp    byte ptr [esi + 155dh], 0
        je     SHORT loc_3_0C52
        mov    al, byte ptr [ebp - 18h]
        cmp    byte ptr [esi + 155ch], al
        jne    SHORT loc_3_0CC4
loc_3_0C52:
        push   dword ptr [ebp - 8]
        call   sub_3_0530
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_0CC4
        cmp    byte ptr [esi + 155dh], 0
        jne    SHORT loc_3_0C89
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_0C89
loc_3_0C73:
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 1554h], ecx
        je     SHORT loc_3_0C85
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_3_0C73
loc_3_0C85:
        test   eax, eax
        jne    SHORT loc_3_0CC4
loc_3_0C89:
        push   2
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0C97:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_0CC4
        push   0
        lea    ecx, [ebp - 14h]
        push   0
        push   2
        push   dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0CB0:
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 14h]
        mov    ax, word ptr [ecx + 4]
        and    ax, 0ff0fh
        cmp    ax, 200h
        je     SHORT loc_3_0CDF
loc_3_0CC4:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0CD2:
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_0C09
        jmp    SHORT loc_3_0D46
loc_3_0CDF:
        push   1
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 2]
        mov    dword ptr [ebp - 0ch], 2
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D02:
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_0D0F
        cmp    byte ptr [ebp - 2], 31h
        jne    SHORT loc_3_0D46
loc_3_0D0F:
        mov    eax, dword ptr [ebp - 14h]
        push   2
        mov    ax, word ptr [eax + 4]
        push   edi
        add    ax, 400h
        lea    ecx, [ebp - 10h]
        mov    word ptr [ebp - 9ah], ax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D2D:
        add    esp, 0ch
        push   0
        push   dword ptr [ebp - 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D3A:
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 1554h], eax
loc_3_0D46:
        cmp    word ptr [ebp - 9ch], 3
        jne    SHORT loc_3_0D7E
        xor    cx, cx
loc_3_0D53:
        movzx  eax, cx
        add    eax, eax
        cmp    word ptr [ebp + eax - 72h], 4
        jne    NEAR PTR loc_3_0E9F
        mov    dx, word ptr [ebp + eax - 9ah]
        inc    cx
        mov    word ptr [esi + eax + 24h], dx
        cmp    cx, 3
        jb     SHORT loc_3_0D53
        jmp    NEAR PTR loc_3_0E1F
loc_3_0D7E:
        cmp    word ptr [ebp - 9ch], 1
        jne    NEAR PTR loc_3_0E9F
        cmp    word ptr [ebp - 72h], 4
        jne    SHORT loc_3_0DC7
        mov    ax, word ptr [ebp - 9ah]
        push   esi
        mov    word ptr [esi + 24h], ax
        add    ax, 400h
        mov    word ptr [esi + 26h], ax
        mov    ax, word ptr [ebp - 9ah]
        add    ax, 800h
        mov    word ptr [esi + 28h], ax
        call   sub_3_0EB4
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_0E1F
        jmp    NEAR PTR loc_3_0E9F
loc_3_0DC7:
        cmp    word ptr [ebp - 9ch], 1
        jne    NEAR PTR loc_3_0E9F
        cmp    word ptr [ebp - 72h], 10h
        jne    NEAR PTR loc_3_0E9F
        mov    ax, word ptr [ebp - 9ah]
        add    ax, 4
        mov    word ptr [esi + 24h], ax
        mov    ax, word ptr [ebp - 9ah]
        add    ax, 8
        mov    word ptr [esi + 26h], ax
        mov    ax, word ptr [ebp - 9ah]
        add    ax, 0ch
        test   byte ptr [ebp - 99h], 80h
        mov    word ptr [esi + 28h], ax
        je     SHORT loc_3_0E1F
        push   esi
        call   sub_3_0EB4
        add    esp, 4
loc_3_0E1F:
        push   esi
        call   sub_3_00E5
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_0E4A
        cmp    dword ptr [ebx], 10h
        jne    SHORT loc_3_0E3A
        push   esi
        call   sub_3_0F98
        add    esp, 4
loc_3_0E3A:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E40:
        add    esp, 4
        mov    eax, 24h
        jmp    SHORT loc_3_0EAD
loc_3_0E4A:
        push   edi
        call   sub_3_0598
        movzx  eax, ax
        add    esp, 4
        mov    dword ptr [esi + 154eh], eax
        push   esi
        call   0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0E9F
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        xor    eax, eax
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [esi + 24h]
        and    eax, 0fff8h
        push   eax
        push   45h
        call   sub_3_03A8
        add    esp, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E92:
        test   eax, eax
        je     SHORT loc_3_0E9B
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E9B:
        xor    eax, eax
        jmp    SHORT loc_3_0EAD
loc_3_0E9F:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0EA5:
        add    esp, 4
loc_3_0EA8:
        mov    eax, 13h
loc_3_0EAD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0EB4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cmp    dword ptr [ebp + 1558h], 10h
        je     SHORT loc_3_0ECF
        mov    eax, 13h
        jmp    NEAR PTR loc_3_0F93
loc_3_0ECF:
        mov    edx, 279h
        xor    al, al
        out    dx, al
        out    dx, al
        xor    ecx, ecx
loc_3_0EDA:
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        mov    edx, 279h
        out    dx, al
        inc    ecx
        cmp    ecx, 20h
        jl     SHORT loc_3_0EDA
        mov    esi, edx
        mov    al, 3
        out    dx, al
        mov    edi, 0a79h
        mov    al, byte ptr [ebp + 155ch]
        mov    edx, edi
        mov    bl, byte ptr [ebp + 155dh]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, 30h
        out    dx, al
        mov    cx, word ptr [ebp + 26h]
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 62h
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 63h
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    cx, word ptr [ebp + 28h]
        mov    edx, esi
        mov    al, 64h
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 65h
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    cx, word ptr [ebp + 24h]
        mov    edx, esi
        mov    al, 66h
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 67h
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 60h
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    edx, esi
        mov    al, 61h
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, esi
        mov    al, 30h
        out    dx, al
        mov    edx, edi
        mov    al, 1
        out    dx, al
        mov    edx, esi
        mov    al, 2
        out    dx, al
        mov    edx, edi
        out    dx, al
        xor    eax, eax
loc_3_0F93:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0F98:
        push   esi
        mov    edx, 279h
        push   edi
        xor    al, al
        out    dx, al
        out    dx, al
        xor    ecx, ecx
loc_3_0FA5:
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        mov    edx, 279h
        out    dx, al
        inc    ecx
        cmp    ecx, 20h
        jl     SHORT loc_3_0FA5
        mov    esi, edx
        mov    al, 3
        mov    edi, 0a79h
        mov    ecx, dword ptr [esp + 0ch]
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ecx + 155ch]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    cl, byte ptr [ecx + 155dh]
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 30h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 62h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 63h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 64h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 65h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 66h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 67h
        out    dx, al
        mov    edx, edi
        xor    al, al
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, 30h
        out    dx, al
        mov    edx, edi
        mov    al, 1
        out    dx, al
        mov    edx, esi
        mov    al, 2
        out    dx, al
        mov    edx, edi
        pop    edi
        out    dx, al
        xor    eax, eax
        pop    esi
        ret
loc_3_1045:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0f0h
        test   eax, eax
        je     SHORT loc_3_1089
        cmp    eax, 1
        je     SHORT loc_3_10A9
        cmp    eax, 2
        je     NEAR PTR loc_3_1172
        cmp    eax, 3
        je     NEAR PTR loc_3_11D2
        cmp    eax, 4
        je     NEAR PTR loc_3_1172
        cmp    eax, 0dh
        je     NEAR PTR loc_3_1172
        mov    eax, 1
        jmp    NEAR PTR loc_3_11DF
loc_3_1089:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_109F
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_3_11DD
loc_3_109F:
        mov    eax, 7
        jmp    NEAR PTR loc_3_11DF
loc_3_10A9:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_3_10B9
        mov    eax, 1
        jmp    NEAR PTR loc_3_11DF
loc_3_10B9:
        push   0
        lea    ecx, [ebp - 0f0h]
        push   dword ptr [ebp + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10CA:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_11DF
        push   dword ptr [ebp + 14h]
        db  068h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp - 92h]
        db  068h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], ax
        lea    edx, [ebp - 4]
        inc    ax
        push   edx
        mov    word ptr [ebp - 2], ax
        push   2
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10FE:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_3_110F
        mov    eax, 13h
        jmp    NEAR PTR loc_3_11DF
loc_3_110F:
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    dword ptr [ebp - 18h], 80h
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0F0h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [ebp - 4]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 04Ah, 004h
        add    esp, 0ch
        movzx  eax, word ptr [ebp - 4]
        push   eax
        push   50h
        call   sub_3_03A8
        add    esp, 8
        jmp    SHORT loc_3_11DD
loc_3_1172:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    word ptr [ebp - 4], ax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        inc    ax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2], ax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   ecx
        push   2
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_11A8:
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        add    esp, 8
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_3_11DF
loc_3_11D2:
        cmp    dword ptr [ebp + 0ch], 1
        mov    eax, 1
        ja     SHORT loc_3_11DF
loc_3_11DD:
        xor    eax, eax
loc_3_11DF:
        mov    esp, ebp
        pop    ebp
        ret
loc_3_11E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    dword ptr [ebp + 8], 5
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_3_11FC
        mov    eax, 1
        jmp    NEAR PTR loc_3_148C
loc_3_11FC:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_120A:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_121B
        mov    eax, 1
        jmp    NEAR PTR loc_3_148C
loc_3_121B:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_122E:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_148A
        db  0A1h, 000h, 000h, 000h, 000h
        test   byte ptr [eax + 25h], 80h
        je     NEAR PTR loc_3_12C8
        mov    si, 2
        db  0BFh, 000h, 000h, 000h, 000h
        db  066h, 0C7h, 005h, 00Eh, 000h, 000h, 000h, 001h, 000h
        xor    ebx, ebx
loc_3_125C:
        push   ebx
        lea    ecx, [ebp - 4]
        push   3000h
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_126E:
        add    esp, 10h
        lea    eax, [esi - 7f30h]
        db  066h, 0A3h, 010h, 000h, 000h, 000h
        push   ebx
        db  066h, 0A3h, 012h, 000h, 000h, 000h
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1292:
        add    esp, 18h
        lea    eax, [esi - 7e30h]
        add    si, 2
        push   ebx
        db  066h, 0A3h, 010h, 000h, 000h, 000h
        push   18h
        db  066h, 0A3h, 012h, 000h, 000h, 000h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_12BA:
        add    esp, 18h
        cmp    si, 0eh
        jbe    SHORT loc_3_125C
        jmp    NEAR PTR loc_3_148A
loc_3_12C8:
        xor    esi, esi
        mov    ebx, 2000h
        push   esi
        lea    ecx, [ebp - 4]
        push   ebx
        push   dword ptr [ebp - 8]
        push   ecx
        db  0BFh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_12E2:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 000h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 001h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1306:
        add    esp, 18h
        lea    ecx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1317:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 030h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 031h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        mov    ebx, 3000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1340:
        add    esp, 18h
        lea    eax, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1351:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 020h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 021h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1375:
        add    esp, 18h
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1386:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 032h, 003h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 037h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 00Ch, 000h, 000h, 000h, 0FEh, 0FFh
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_13B3:
        add    esp, 18h
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_13C4:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 040h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 041h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_13E8:
        add    esp, 18h
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_13F9:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 050h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 051h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_141D:
        add    esp, 18h
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_142E:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 060h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 061h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1452:
        add    esp, 18h
        lea    edx, [ebp - 4]
        push   esi
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1463:
        db  066h, 0C7h, 005h, 010h, 000h, 000h, 000h, 070h, 003h
        add    esp, 10h
        db  066h, 0C7h, 005h, 012h, 000h, 000h, 000h, 071h, 003h
        push   esi
        push   18h
        push   edi
        push   2
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1487:
        add    esp, 18h
loc_3_148A:
        xor    eax, eax
loc_3_148C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1493:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_14D8
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_14C4
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        push   ecx
        push   3
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
        jmp    SHORT loc_3_1536
loc_3_14C4:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        push   eax
        push   50h
        call   sub_3_03A8
        add    esp, 8
        jmp    SHORT loc_3_1536
loc_3_14D8:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_14F4
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_14F4
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_14F1:
        add    esp, 4
loc_3_14F4:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_1513
        push   eax
        push   eax
        push   4
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 10h
        jmp    SHORT loc_3_1536
loc_3_1513:
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
loc_3_1536:
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
sub_3_153C:
        push   dword ptr [esp + 4]
        call   sub_3_1548
        ret    8
sub_3_1548:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        xor    esi, esi
        mov    ax, word ptr [edi + 4]
        add    ax, 2
        movzx  edx, ax
        inc    edx
        in     al, dx
        mov    cl, al
loc_3_1564:
        in     al, dx
        cmp    al, cl
        jne    SHORT loc_3_156F
        inc    esi
        cmp    esi, 20h
        jb     SHORT loc_3_1564
loc_3_156F:
        cmp    esi, 20h
        je     NEAR PTR loc_3_15FD
        push   4e0h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1583:
        and    al, 8ch
        cmp    al, 0ch
        jne    SHORT loc_3_15FD
        mov    ebx, 23eh
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1595:
        movzx  bp, al
        not    bp
        and    bp, 6
        push   23fh
        movzx  esi, bp
        shl    esi, 4
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_15B1:
        xor    cx, cx
        mov    cl, al
        and    cx, 1fh
        mov    word ptr [esp + 12h], cx
        mov    ecx, esi
        movzx  eax, word ptr [esp + 12h]
        or     ecx, eax
        push   ecx
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_15D0:
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_15D7:
        xor    ecx, ecx
        mov    cl, al
        movzx  eax, bp
        and    ecx, 6
        cmp    ecx, eax
        jne    SHORT loc_3_15FD
        not    esi
        movzx  eax, word ptr [esp + 12h]
        and    esi, 60h
        or     esi, eax
        push   esi
        push   ebx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_15F9:
        xor    eax, eax
        jmp    SHORT loc_3_1602
loc_3_15FD:
        mov    eax, 1
loc_3_1602:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_3_160C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [ebp - 10h], 240h
        mov    dword ptr [ebp - 0ch], 340h
        mov    dword ptr [ebp - 8], 260h
        mov    dword ptr [ebp - 4], 360h
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_3_1635:
        db  00Fh, 0B7h, 004h, 07Dh, 000h, 000h, 000h, 000h
        push   eax
        mov    ecx, edi
        mov    eax, edi
        and    ecx, 1fh
        and    al, 0e7h
        inc    edi
        shr    eax, 3
        mov    eax, dword ptr [ebp + eax - 10h]
        or     eax, ecx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1658:
        cmp    edi, 80h
        jb     SHORT loc_3_1635
        push   400h
        xor    edi, edi
        push   esi
        call   sub_3_16D1
loc_3_166D:
        test   edi, 1
        jne    SHORT loc_3_1680
        db  00Fh, 0B7h, 004h, 07Dh, 000h, 000h, 000h, 000h
        push   eax
        jmp    SHORT loc_3_168F
loc_3_1680:
        db  066h, 08Bh, 004h, 07Dh, 000h, 000h, 000h, 000h
        or     ah, 80h
        movzx  ecx, ax
        push   ecx
loc_3_168F:
        mov    eax, edi
        mov    ecx, edi
        and    al, 0e7h
        and    ecx, 1fh
        shr    eax, 3
        inc    edi
        mov    eax, dword ptr [ebp + eax - 10h]
        or     eax, ecx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16A9:
        cmp    edi, 80h
        jb     SHORT loc_3_166D
        xor    edi, edi
loc_3_16B3:
        push   0
        mov    eax, edi
        or     eax, 220h
        inc    edi
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16C4:
        cmp    edi, 14h
        jb     SHORT loc_3_16B3
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_16D1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   33bh
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16E7:
        mov    esi, dword ptr [ebp + 0ch]
        xor    edi, edi
        add    si, ax
loc_3_16EF:
        push   33bh
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16FC:
        cmp    ax, si
        mov    bx, ax
        je     SHORT loc_3_1732
        mov    dword ptr [ebp - 4], edi
loc_3_1707:
        push   33bh
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1714:
        cmp    ax, bx
        jne    SHORT loc_3_1729
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 4000h
        lea    ecx, [eax + 1]
        mov    dword ptr [ebp - 4], ecx
        jb     SHORT loc_3_1707
loc_3_1729:
        cmp    dword ptr [ebp - 4], 4000h
        jb     SHORT loc_3_16EF
loc_3_1732:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_173B:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   0e0ch
        push   4e0h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1751:
        push   59h
        push   23dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_175E:
        push   20h
        push   23eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_176B:
        push   0
        push   23fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1778:
        push   esi
        call   sub_3_184B
        push   esi
        call   sub_3_1953
        push   esi
        call   sub_3_160C
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_3_1796
        push   esi
        call   sub_3_1A39
loc_3_1796:
        mov    dword ptr [esi + 14h], 5050204h
        mov    word ptr [esi + 12h], 0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17A9:
        add    esp, 4
        push   -1
        push   -1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17B6:
        add    esp, 0ch
        cmp    word ptr [esi + 6], 1eh
        ja     SHORT loc_3_17C6
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17C6:
        push   6
        push   23fh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17D3:
        push   23eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_17DE:
        test   al, 40h
        jne    SHORT loc_3_17E9
        mov    eax, 1
        jmp    SHORT loc_3_1846
loc_3_17E9:
        mov    eax, 20h
        lea    ecx, [esi + 42h]
        xor    edx, edx
        mov    dword ptr [esi + 18h], eax
loc_3_17F6:
        mov    word ptr [ecx], 0ffeh
        add    ecx, 20h
        dec    eax
        mov    byte ptr [ecx - 0ch], dl
        mov    dword ptr [ecx - 16h], edx
        mov    byte ptr [ecx - 9], 1
        jne    SHORT loc_3_17F6
        lea    edi, [esi + 662h]
        xor    eax, eax
        mov    ecx, 120h
        xor    edx, edx
        rep stosd
        mov    eax, 80h
        lea    edi, [esi + 0aeeh]
        mov    ecx, edi
loc_3_1829:
        mov    dword ptr [ecx], edx
        add    ecx, 8
        dec    eax
        jne    SHORT loc_3_1829
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_1844
        mov    dword ptr [edi], 200000h
        mov    dword ptr [esi + 0af2h], eax
loc_3_1844:
        xor    eax, eax
loc_3_1846:
        pop    edi
        pop    esi
        ret    4
sub_3_184B:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        xor    edi, edi
        xor    ebx, ebx
loc_3_1856:
        push   80h
        mov    eax, edi
        or     eax, 2a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1869:
        push   ebx
        mov    eax, edi
        or     eax, 3c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1878:
        push   ebx
        mov    eax, edi
        or     eax, 2e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1887:
        push   0d000h
        mov    eax, edi
        or     ah, 4
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1898:
        push   0ff00h
        mov    eax, edi
        or     eax, 420h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18AB:
        push   ebx
        mov    eax, edi
        or     eax, 440h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18BA:
        push   ebx
        mov    eax, edi
        or     eax, 460h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18C9:
        push   18h
        mov    eax, edi
        or     eax, 480h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18D9:
        push   18h
        mov    eax, edi
        or     eax, 4a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18E9:
        push   ebx
        mov    eax, edi
        or     eax, 4c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18F8:
        push   ebx
        mov    eax, edi
        or     eax, 3e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1907:
        push   ebx
        mov    eax, edi
        or     eax, 3a0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1916:
        push   ebx
        mov    eax, edi
        or     eax, 380h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1925:
        push   ebx
        mov    eax, edi
        or     eax, 280h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1934:
        push   ebx
        mov    eax, edi
        or     eax, 2c0h
        inc    edi
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1944:
        cmp    edi, 20h
        jb     NEAR PTR loc_3_1856
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_1953:
        push   ebx
        push   esi
        push   edi
        mov    esi, 80000000h
        mov    ebx, dword ptr [esp + 10h]
        push   ebp
        push   esi
        push   234h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_196C:
        push   esi
        push   235h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1978:
        push   esi
        push   236h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1984:
        push   esi
        push   237h
        push   ebx
        xor    esi, esi
        mov    edi, 0ffffh
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1997:
        push   2
        mov    ebp, 1d8h
        push   ebx
        call   sub_3_16D1
loc_3_19A4:
        push   0
        mov    eax, esi
        or     al, 20h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19B1:
        push   edi
        mov    eax, esi
        or     al, 60h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19BD:
        push   ebp
        mov    eax, esi
        or     al, 0c0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19C9:
        push   1e0h
        mov    eax, esi
        or     al, 0e0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19D9:
        push   0
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19E2:
        push   edi
        mov    eax, esi
        or     al, 40h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19EE:
        push   ebp
        mov    eax, esi
        or     ah, 2
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_19FB:
        push   0
        mov    eax, esi
        or     al, 0a0h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A08:
        push   0
        mov    eax, esi
        or     al, 80h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A15:
        push   807fh
        mov    eax, esi
        or     eax, 2a0h
        inc    esi
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A29:
        cmp    esi, 20h
        jb     NEAR PTR loc_3_19A4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_1A39:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        xor    edi, edi
        mov    esi, dword ptr [esp + 18h]
loc_3_1A48:
        push   80h
        mov    eax, edi
        or     eax, 2a0h
        inc    edi
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A5C:
        cmp    edi, 1eh
        jb     SHORT loc_3_1A48
        xor    edi, edi
        mov    ebp, 40000000h
loc_3_1A68:
        push   0
        mov    eax, edi
        or     al, 60h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A75:
        push   0
        mov    eax, edi
        or     al, 40h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A82:
        push   0
        mov    eax, edi
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A8F:
        push   0
        mov    eax, edi
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A9C:
        push   ebp
        mov    eax, edi
        or     al, 20h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AA8:
        push   ebp
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AB0:
        test   edi, 1
        je     SHORT loc_3_1ABF
        push   6000000h
        jmp    SHORT loc_3_1AC4
loc_3_1ABF:
        push   4000000h
loc_3_1AC4:
        mov    eax, edi
        inc    edi
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AD1:
        cmp    edi, 1eh
        jb     SHORT loc_3_1A68
        push   200000h
        push   236h
        push   esi
        mov    edi, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AEB:
        push   0aa55h
        push   23ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1AFB:
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B06:
        test   eax, 80000000h
        je     SHORT loc_3_1B10
        dec    edi
        jne    SHORT loc_3_1AFB
loc_3_1B10:
        mov    edi, 200000h
        mov    ebp, 23ah
        push   edi
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B26:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B2D:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B34:
        cmp    ax, 0aa55h
        jne    NEAR PTR loc_3_1C3F
        push   edi
        push   236h
        push   esi
        mov    edi, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B4F:
        push   55aah
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B5B:
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B66:
        test   eax, 80000000h
        je     SHORT loc_3_1B70
        dec    edi
        jne    SHORT loc_3_1B5B
loc_3_1B70:
        push   200000h
        mov    ebp, 23ah
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B85:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B8C:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1B93:
        cmp    ax, 55aah
        jne    NEAR PTR loc_3_1C3F
        mov    ebx, 40000h
loc_3_1BA2:
        lea    edi, [ebx + 200000h]
        push   edi
        push   236h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1BB4:
        push   0aa55h
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1BC0:
        push   200000h
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1BD0:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1BD7:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1BDE:
        cmp    ax, 55aah
        jne    SHORT loc_3_1C32
        mov    dword ptr [esp + 10h], 80h
loc_3_1BEC:
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1BF7:
        test   eax, 80000000h
        je     SHORT loc_3_1C04
        dec    dword ptr [esp + 10h]
        jne    SHORT loc_3_1BEC
loc_3_1C04:
        push   edi
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C10:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C17:
        push   ebp
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C1E:
        cmp    ax, 0aa55h
        jne    SHORT loc_3_1C32
        add    ebx, ebx
        cmp    ebx, 1000000h
        jb     NEAR PTR loc_3_1BA2
loc_3_1C32:
        cmp    ebx, 1000000h
        jne    SHORT loc_3_1C3F
        mov    ebx, 0dfffe0h
loc_3_1C3F:
        mov    edi, 80h
        mov    dword ptr [esi + 8], ebx
        mov    dword ptr [esi + 0ch], ebx
loc_3_1C4A:
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C55:
        test   eax, 80000000h
        je     SHORT loc_3_1C5F
        dec    edi
        jne    SHORT loc_3_1C4A
loc_3_1C5F:
        xor    ebx, ebx
        mov    edi, 23ah
        push   ebx
        push   234h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C72:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C79:
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C80:
        and    ax, 0ffh
        push   edi
        sub    ax, 30h
        push   esi
        mov    word ptr [esi + 1526h], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C96:
        and    ax, 0ffh
        push   edi
        sub    ax, 30h
        push   esi
        mov    word ptr [esi + 1528h], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CAC:
        push   edi
        mov    byte ptr [esi + 151eh], ah
        push   esi
        mov    byte ptr [esi + 151fh], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CBF:
        mov    byte ptr [esi + 1520h], ah
        mov    byte ptr [esi + 1521h], al
        mov    byte ptr [esi + 1522h], bl
loc_3_1CD1:
        push   0
        mov    eax, ebx
        or     ah, 2
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CDF:
        push   1d8h
        mov    eax, ebx
        or     al, 0c0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CEF:
        push   1e0h
        mov    eax, ebx
        or     al, 0e0h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CFF:
        push   40000000h
        mov    eax, ebx
        or     al, 20h
        inc    ebx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1D10:
        cmp    ebx, 1eh
        jb     SHORT loc_3_1CD1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  0CCh
sub_3_1D20:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
_PTEXT ends
        end
