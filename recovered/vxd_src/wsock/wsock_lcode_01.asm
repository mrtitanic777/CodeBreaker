; wsock_lcode_01.asm -- WSOCK object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  053h, 052h, 056h, 08Bh, 054h, 024h, 010h, 08Bh, 05Ah, 00Ch, 08Bh, 0F3h, 003h, 035h, 08Ch, 022h
        db  000h, 000h, 0F7h, 046h, 008h, 001h, 000h, 000h, 000h, 075h, 009h, 089h, 036h, 089h, 076h, 004h
        db  083h, 04Eh, 008h, 001h, 089h, 032h, 08Bh, 046h, 004h, 089h, 042h, 004h, 089h, 010h, 089h, 056h
        db  004h, 0F7h, 046h, 008h, 002h, 000h, 000h, 000h, 075h, 01Bh, 083h, 04Eh, 008h, 002h, 0B8h, 000h
        db  010h, 000h, 000h, 0B9h, 003h, 000h, 000h, 000h, 08Bh, 0D6h, 0BEh, 059h, 000h, 000h, 000h, 0CDh
        db  020h, 014h, 000h, 001h, 000h, 05Eh, 05Ah, 05Bh, 0C3h
loc_1_0059:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    edi, edx
loc_1_0070:
        mov    edx, dword ptr [edi]
        cmp    edx, edi
        je     SHORT loc_1_0098
        call   sub_1_00C6
        jb     SHORT loc_1_00B2
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        db  0FFh, 035h, 0E4h, 021h, 000h, 000h
        call   sub_1_0C17
        add    esp, 8
        jmp    SHORT loc_1_0070
loc_1_0098:
        and    dword ptr [edi + 8], 0fffffffdh
loc_1_009C:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
loc_1_00B2:
        mov    eax, 1f4h
        mov    edx, edi
        db  0BEh, 059h, 000h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        jmp    SHORT loc_1_009C
sub_1_00C6:
        push   edx
        mov    eax, dword ptr [edx + 8]
        db  03Bh, 005h, 088h, 022h, 000h, 000h
        je     SHORT loc_1_0113
        mov    eax, dword ptr [edx + 10h]
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 14h]
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    cx, word ptr [edx + 0ah]
        movzx  edx, word ptr [edx + 8]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        clc
        pop    edx
        ret
loc_1_0113:
        movzx  eax, word ptr [edx + 14h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        movzx  eax, word ptr [edx + 16h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 18h]
        mov    ecx, eax
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [edx + 10h]
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, ecx
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    cx, word ptr [edx + 0ah]
        movzx  edx, word ptr [edx + 8]
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 1
        pop    edx
        ret
        db  000h, 000h, 000h
loc_1_0170:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        mov    eax, esi
        jne    SHORT loc_1_0180
        call   sub_1_0BB2
loc_1_0180:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        mov    esi, dword ptr [esp + 0ch]
        push   eax
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_01AC
        add    esp, 18h
        mov    dword ptr [esi + 10h], eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 8
        pop    esi
        ret
sub_1_01AC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0825
        mov    edi, dword ptr [esp + 1ch]
        cmp    edi, -1
        mov    esi, dword ptr [esp + 20h]
        jne    SHORT loc_1_01E7
        movzx  eax, si
        push   eax
        shr    esi, 10h
        movzx  eax, si
        mov    ebp, dword ptr [esp + 28h]
        push   eax
        push   ebp
        call   sub_1_0B2C
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_01EB
        sub    eax, eax
        jmp    NEAR PTR loc_1_029B
loc_1_01E7:
        mov    ebp, dword ptr [esp + 24h]
loc_1_01EB:
        db  081h, 03Dh, 0E8h, 021h, 000h, 000h, 0E8h, 021h, 000h, 000h
        je     SHORT loc_1_0222
        db  0A1h, 0E8h, 021h, 000h, 000h
        db  08Bh, 01Dh, 0E8h, 021h, 000h, 000h
        mov    ecx, dword ptr [eax]
        sub    ebx, 8
        db  0C7h, 041h, 004h, 0E8h, 021h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 0E8h, 021h, 000h, 000h
        db  0A1h, 070h, 022h, 000h, 000h
        dec    eax
        db  0A3h, 070h, 022h, 000h, 000h
        jmp    SHORT loc_1_022E
loc_1_0222:
        push   2ch
        call   sub_1_0C28
        add    esp, 4
        mov    ebx, eax
loc_1_022E:
        or     ebx, ebx
        je     SHORT loc_1_0299
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 10h], ecx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebx + 14h], eax
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 18h], edx
        mov    dword ptr [ebx + 20h], ebp
        mov    dword ptr [ebx + 24h], edi
        mov    dword ptr [ebx + 28h], esi
        db  0C7h, 043h, 008h, 074h, 022h, 000h, 000h
        lea    esi, [ebx + 8]
        db  0A1h, 078h, 022h, 000h, 000h
        mov    dword ptr [ebx + 0ch], eax
        db  0A1h, 078h, 022h, 000h, 000h
        mov    dword ptr [eax], esi
        lea    eax, [ecx + 8]
        db  089h, 035h, 078h, 022h, 000h, 000h
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [eax], ebx
        mov    dword ptr [ecx + 0ch], ebx
        mov    eax, dword ptr [ecx + 54h]
        not    eax
        and    eax, dword ptr [ecx + 50h]
        test   edx, eax
        je     SHORT loc_1_0299
        push   0
        push   ebx
        call   sub_1_0637
        add    esp, 8
loc_1_0299:
        mov    eax, ebx
loc_1_029B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_02A0:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        or     esi, esi
        mov    eax, esi
        jne    SHORT loc_1_02B0
        call   sub_1_0BB2
loc_1_02B0:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 1ch]
        push   dword ptr [eax + 18h]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 14h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 10h]
        push   dword ptr [eax]
        push   dword ptr [eax + 0ch]
        call   sub_1_02DD
        add    esp, 28h
        pop    esi
        ret
sub_1_02DD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ecx, dword ptr [esp + 14h]
        cmp    ecx, eax
        je     SHORT loc_1_030E
        mov    ebp, dword ptr [esp + 38h]
        mov    ebx, dword ptr [esp + 34h]
        push   ebp
        mov    edi, dword ptr [esp + 34h]
        push   ebx
        mov    esi, dword ptr [esp + 34h]
        push   edi
        push   esi
        push   dword ptr [esp + 28h]
        push   ecx
        call   sub_1_06FF
        add    esp, 18h
        jmp    SHORT loc_1_031E
loc_1_030E:
        mov    esi, dword ptr [esp + 2ch]
        mov    edi, dword ptr [esp + 30h]
        mov    ebx, dword ptr [esp + 34h]
        mov    ebp, dword ptr [esp + 38h]
loc_1_031E:
        mov    ecx, dword ptr [esp + 1ch]
        or     ecx, ecx
        je     SHORT loc_1_033B
        or     eax, eax
        jne    SHORT loc_1_033B
        push   ebp
        push   ebx
        push   edi
        push   esi
        push   dword ptr [esp + 30h]
        push   ecx
        call   sub_1_06FF
        add    esp, 18h
loc_1_033B:
        mov    ecx, dword ptr [esp + 24h]
        or     ecx, ecx
        je     SHORT loc_1_0358
        or     eax, eax
        jne    SHORT loc_1_0358
        push   ebp
        push   ebx
        push   edi
        push   esi
        push   dword ptr [esp + 38h]
        push   ecx
        call   sub_1_06FF
        add    esp, 18h
loc_1_0358:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_035D:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax]
        call   sub_1_036E
        add    esp, 4
        sub    eax, eax
        ret
sub_1_036E:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx + 8]
        mov    esi, dword ptr [edx + 0ch]
        lea    ecx, [edx + 8]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        test   byte ptr [edx + 14h], 8
        jne    SHORT loc_1_039B
        mov    esi, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
loc_1_039B:
        db  08Bh, 035h, 070h, 022h, 000h, 000h
        cmp    esi, 40h
        jae    SHORT loc_1_03CA
        db  0C7h, 001h, 0E8h, 021h, 000h, 000h
        db  0A1h, 0ECh, 021h, 000h, 000h
        mov    dword ptr [edx + 0ch], eax
        db  0A1h, 0ECh, 021h, 000h, 000h
        inc    esi
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 0ECh, 021h, 000h, 000h
        db  089h, 035h, 070h, 022h, 000h, 000h
        jmp    SHORT loc_1_03D3
loc_1_03CA:
        push   edx
        call   sub_1_0C2E
        add    esp, 4
loc_1_03D3:
        sub    eax, eax
        pop    esi
        ret
loc_1_03D7:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [eax]
        mov    eax, esi
        mov    edi, dword ptr [esi + 8]
        sub    eax, edi
        cmp    eax, -8
        je     SHORT loc_1_0401
loc_1_03EB:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_036E
        add    esp, 4
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -8
        jne    SHORT loc_1_03EB
loc_1_0401:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_0406:
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        jne    SHORT loc_1_0413
        call   sub_1_0BB2
loc_1_0413:
        push   eax
        call   sub_1_041F
        add    esp, 4
        sub    eax, eax
        ret
sub_1_041F:
        push   dword ptr [esp + 4]
        call   sub_1_06C4
        add    esp, 4
        sub    eax, eax
        ret
loc_1_042E:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_1_0445
        add    esp, 0ch
        sub    eax, eax
        ret
sub_1_0445:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        cmp    edi, 4
        jne    SHORT loc_1_0466
        mov    esi, dword ptr [esp + 18h]
        test   byte ptr [esi + 40h], 10h
        je     SHORT loc_1_0488
        mov    edi, 1
        jmp    SHORT loc_1_0488
loc_1_0466:
        cmp    edi, 10h
        jne    SHORT loc_1_0484
        mov    ebx, dword ptr [esp + 20h]
        or     ebx, ebx
        mov    esi, dword ptr [esp + 18h]
        je     SHORT loc_1_048C
        cmp    dword ptr [esi + 58h], 0
        jne    SHORT loc_1_048C
        mov    edi, 100h
        jmp    SHORT loc_1_048C
loc_1_0484:
        mov    esi, dword ptr [esp + 18h]
loc_1_0488:
        mov    ebx, dword ptr [esp + 20h]
loc_1_048C:
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [esp + 10h], eax
        mov    ecx, edi
        or     ecx, eax
        mov    dword ptr [esi + 50h], ecx
        mov    eax, dword ptr [esi + 54h]
        not    eax
        and    edi, eax
        mov    ebp, dword ptr [esi + 8]
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, -8
        je     SHORT loc_1_04DB
loc_1_04AD:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_1_04C3
        push   eax
        call   sub_1_036E
        add    esp, 4
        jmp    SHORT loc_1_04D2
loc_1_04C3:
        test   dword ptr [eax + 18h], edi
        je     SHORT loc_1_04D2
        push   ebx
        push   eax
        call   sub_1_0637
        add    esp, 8
loc_1_04D2:
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, -8
        jne    SHORT loc_1_04AD
loc_1_04DB:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_0532
        test   dword ptr [esi + 60h], edi
        je     SHORT loc_1_0532
        test   dword ptr [esp + 10h], edi
        jne    SHORT loc_1_0532
        or     ebx, ebx
        jne    SHORT loc_1_04FA
        mov    eax, edi
        and    eax, 30h
        or     dword ptr [esi + 54h], eax
        jmp    SHORT loc_1_0503
loc_1_04FA:
        cmp    edi, 10h
        jne    SHORT loc_1_0503
        and    dword ptr [esi + 50h], 0ffffffefh
loc_1_0503:
        push   ebx
        push   edi
        push   esi
        call   sub_1_05A6
        add    esp, 0ch
        cmp    edi, 10h
        jne    SHORT loc_1_0532
        or     ebx, ebx
        jne    SHORT loc_1_0532
        and    dword ptr [esi + 54h], 0fffffffdh
        or     dword ptr [esi + 50h], 2
        test   byte ptr [esi + 60h], 2
        je     SHORT loc_1_0532
        push   0
        push   2
        push   esi
        call   sub_1_05A6
        add    esp, 0ch
loc_1_0532:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_053A:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_1_054E
        add    esp, 8
        sub    eax, eax
        ret
sub_1_054E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        mov    eax, edi
        mov    ebx, dword ptr [edi + 8]
        sub    eax, ebx
        cmp    eax, -8
        je     SHORT loc_1_0594
        mov    esi, dword ptr [esp + 18h]
        sub    ebp, ebp
loc_1_0568:
        mov    eax, ebx
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [eax + 24h], ebp
        jne    SHORT loc_1_057C
        push   eax
        call   sub_1_036E
        add    esp, 4
        jmp    SHORT loc_1_058B
loc_1_057C:
        cmp    dword ptr [eax + 18h], ebp
        je     SHORT loc_1_058B
        push   esi
        push   eax
        call   sub_1_0637
        add    esp, 8
loc_1_058B:
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -8
        jne    SHORT loc_1_0568
loc_1_0594:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0599:
        push   dword ptr [esp + 4]
        call   sub_1_06C4
        add    esp, 4
        ret
sub_1_05A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 58h], -1
        jne    SHORT loc_1_05D0
        mov    eax, dword ptr [eax + 5ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        jmp    SHORT loc_1_0632
loc_1_05D0:
        db  0FFh, 035h, 0E4h, 021h, 000h, 000h
        call   sub_1_0C08
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_0632
        db  08Bh, 00Dh, 088h, 022h, 000h, 000h
        mov    dword ptr [eax + 8], ecx
        xor    ebx, ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], ebx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 58h]
        mov    word ptr [eax + 14h], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 5ch]
        mov    word ptr [eax + 16h], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 68h]
        mov    dword ptr [eax + 18h], ecx
        movzx  ecx, word ptr [ebp + 10h]
        shl    ecx, 10h
        movzx  edx, word ptr [ebp + 0ch]
        or     ecx, edx
        mov    dword ptr [eax + 10h], ecx
        push   eax
        call   0
        add    esp, 4
loc_1_0632:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_0637:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [esi + 1ch], edi
        mov    eax, dword ptr [esi + 14h]
        test   al, 6
        je     SHORT loc_1_0657
        push   edi
        push   esi
        call   sub_1_07A1
        add    esp, 8
        jmp    SHORT loc_1_06C1
loc_1_0657:
        test   al, 1
        jne    SHORT loc_1_0677
        push   esi
        push   dword ptr [esi + 24h]
        push   dword ptr [esi + 20h]
        call   sub_1_0B9C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_06C1
        mov    dword ptr [esi + 18h], 0
        jmp    SHORT loc_1_06C1
loc_1_0677:
        cmp    dword ptr [esi + 24h], -1
        jne    SHORT loc_1_0688
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [eax], edi
        mov    byte ptr [eax + 4], 1
        jmp    SHORT loc_1_06B8
loc_1_0688:
        db  0FFh, 035h, 0E4h, 021h, 000h, 000h
        call   sub_1_0C08
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_06C1
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [eax + 10h], ecx
        mov    dword ptr [eax + 14h], edi
        push   eax
        call   0
        add    esp, 4
loc_1_06B8:
        push   esi
        call   sub_1_036E
        add    esp, 4
loc_1_06C1:
        pop    edi
        pop    esi
        ret
sub_1_06C4:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 03Dh, 074h, 022h, 000h, 000h
        db  081h, 0FFh, 074h, 022h, 000h, 000h
        je     SHORT loc_1_06FB
        mov    esi, dword ptr [esp + 10h]
        sub    ebx, ebx
loc_1_06DB:
        lea    eax, [edi - 8]
        mov    edi, dword ptr [edi]
        cmp    dword ptr [eax + 24h], ebx
        je     SHORT loc_1_06EA
        cmp    dword ptr [eax + 20h], esi
        jne    SHORT loc_1_06F3
loc_1_06EA:
        push   eax
        call   sub_1_036E
        add    esp, 4
loc_1_06F3:
        db  081h, 0FFh, 074h, 022h, 000h, 000h
        jne    SHORT loc_1_06DB
loc_1_06FB:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_06FF:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, edi
        imul   ebx, ebx, 0ch
        push   ebx
        push   dword ptr [esp + 20h]
        call   sub_1_0A54
        mov    dword ptr [esp + 24h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0733
        mov    eax, 271eh
        jmp    SHORT loc_1_0799
loc_1_0733:
        mov    esi, dword ptr [esp + 1ch]
        sub    ebp, ebp
        cmp    edi, ebp
        je     SHORT loc_1_0788
loc_1_073D:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_01AC
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_0768
        add    esi, 0ch
        inc    ebp
        cmp    edi, ebp
        ja     SHORT loc_1_073D
        jmp    SHORT loc_1_0788
loc_1_0768:
        or     ebp, ebp
        je     SHORT loc_1_0780
loc_1_076C:
        mov    eax, dword ptr [esi - 0ch]
        sub    esi, 0ch
        push   dword ptr [eax + 0ch]
        call   sub_1_036E
        add    esp, 4
        dec    ebp
        jne    SHORT loc_1_076C
loc_1_0780:
        mov    dword ptr [esp + 10h], 2747h
loc_1_0788:
        push   ebx
        push   dword ptr [esp + 20h]
        call   sub_1_0A9B
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
loc_1_0799:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_07A1:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 20h]
        test   byte ptr [esi + 14h], 4
        je     SHORT loc_1_07F4
        or     ebx, ebx
        jne    SHORT loc_1_07E6
        mov    eax, dword ptr [esi + 10h]
        lea    ecx, [esp + 0ch]
        push   ecx
        push   0
        push   0
        push   80000000h
        push   dword ptr [esi + 28h]
        push   dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [eax + 1ch]
        call   dword ptr [eax + 1ch]
        add    esp, 1ch
        mov    ebx, eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [edi + 4], eax
loc_1_07E6:
        push   dword ptr [esi + 28h]
        push   dword ptr [esi + 24h]
        call   sub_1_0A9B
        add    esp, 8
loc_1_07F4:
        or     ebx, ebx
        je     SHORT loc_1_07FF
        mov    dword ptr [edi + 4], 0
loc_1_07FF:
        mov    ebx, dword ptr [edi]
        push   14h
        push   edi
        call   sub_1_0A9B
        add    esp, 8
        push   esi
        call   sub_1_036E
        add    esp, 4
        push   ebx
        call   sub_1_092C
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_0825:
        push   esi
        push   edi
        db  08Bh, 035h, 074h, 022h, 000h, 000h
        db  081h, 0FEh, 074h, 022h, 000h, 000h
        je     SHORT loc_1_0852
        sub    edi, edi
loc_1_0837:
        lea    eax, [esi - 8]
        mov    esi, dword ptr [esi]
        cmp    dword ptr [eax + 24h], edi
        jne    SHORT loc_1_084A
        push   eax
        call   sub_1_036E
        add    esp, 4
loc_1_084A:
        db  081h, 0FEh, 074h, 022h, 000h, 000h
        jne    SHORT loc_1_0837
loc_1_0852:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h
WSOCK_Service_00:
        mov    eax, 100h
        clc
        ret
WSOCK_Control:
        cmp    eax, 1
        db  00Fh, 084h, 044h, 0F9h, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0BBh, 0F9h, 0FEh, 0FFh
        cmp    eax, 21h
        je     SHORT loc_1_087D
        cmp    eax, 23h
        je     SHORT loc_1_0887
        clc
        ret
loc_1_087D:
        push   edi
        call   sub_1_0599
        add    esp, 4
        ret
loc_1_0887:
        push   ebx
        push   esi
        push   edi
        db  083h, 005h, 0A8h, 00Ch, 000h, 000h, 001h
        jne    SHORT loc_1_0899
        db  0CDh, 020h, 011h, 000h, 040h, 000h
loc_1_0899:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 100h
        jb     SHORT loc_1_08FC
        cmp    ecx, 11bh
        ja     SHORT loc_1_08FC
loc_1_08AC:
        db  08Bh, 03Ch, 08Dh, 0ACh, 008h, 000h, 000h
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_08CA
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0925
loc_1_08CA:
        mov    dword ptr [esi + 10h], eax
        push   edx
        mov    edx, 0ffffffffh
        cmp    edx, dword ptr [esi + 4]
        je     SHORT loc_1_08DA
        xor    edx, edx
loc_1_08DA:
        push   edx
        push   eax
        call   edi
        add    esp, 8
        pop    edx
        push   eax
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        call   sub_1_0A9B
        add    esp, 8
        pop    eax
loc_1_08F2:
        db  0FFh, 00Dh, 0A8h, 00Ch, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_08FC:
        cmp    ecx, 0
        je     SHORT loc_1_090D
        cmp    ecx, -1
        je     SHORT loc_1_0916
        mov    eax, 1
        jmp    SHORT loc_1_08F2
loc_1_090D:
        xor    eax, eax
        mov    ecx, 0
        jmp    SHORT loc_1_08F2
loc_1_0916:
        push   dword ptr [esi + 2ch]
        call   sub_1_1C73
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_1_08F2
loc_1_0925:
        mov    eax, 6f8h
        jmp    SHORT loc_1_08F2
sub_1_092C:
        push   ebx
        push   edx
        push   esi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_1_0952
        db  0BEh, 060h, 009h, 000h, 000h
        mov    edx, dword ptr [esp + 10h]
        db  0CDh, 020h, 011h, 000h, 001h, 000h
        jmp    SHORT loc_1_095C
loc_1_0952:
        mov    eax, dword ptr [esp + 10h]
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
loc_1_095C:
        pop    esi
        pop    edx
        pop    ebx
        ret
loc_1_0960:
        mov    eax, edx
        db  0CDh, 020h, 00Eh, 000h, 02Ah, 000h
        ret
WSOCK_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        db  083h, 005h, 0A8h, 00Ch, 000h, 000h, 001h
        jne    SHORT loc_1_097E
        push   ebx
        db  0CDh, 020h, 011h, 000h, 040h, 000h
        pop    ebx
loc_1_097E:
        movzx  eax, word ptr [ebp + 10h]
        push   eax
        movzx  eax, word ptr [ebp + 38h]
        push   eax
        push   ebx
        call   sub_1_0B2C
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0A0D
        or     eax, eax
        je     SHORT loc_1_09AC
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   eax
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0A0D
loc_1_09AC:
        mov    esi, eax
        mov    edx, eax
        movzx  ecx, word ptr [ebp + 1ch]
        db  08Bh, 03Ch, 08Dh, 0ACh, 008h, 000h, 000h
        db  00Fh, 0B6h, 089h, 01Ch, 00Ch, 000h, 000h
        jecxz  loc_1_09E3
loc_1_09C4:
        movzx  eax, word ptr [esi]
        push   eax
        movzx  eax, word ptr [esi + 2]
        push   eax
        push   ebx
        call   sub_1_0B2C
        add    esp, 0ch
        mov    dword ptr [esi], eax
        cmp    eax, -1
        je     SHORT loc_1_0A0D
        add    esi, 4
        dec    ecx
        jne    SHORT loc_1_09C4
loc_1_09E3:
        mov    esi, edx
        push   ebx
        push   edx
        call   edi
        add    esp, 8
        mov    word ptr [ebp + 1ch], ax
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   esi
        call   sub_1_0A9B
        add    esp, 8
        and    dword ptr [ebp + 2ch], 0fffffffeh
loc_1_0A02:
        db  0FFh, 00Dh, 0A8h, 00Ch, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_0A0D:
        mov    word ptr [ebp + 1ch], 271eh
        jmp    SHORT loc_1_0A02
WSOCK_Service_01:
        test   eax, eax
        je     SHORT loc_1_0A1B
        xor    edx, edx
loc_1_0A1B:
        push   edx
        push   ecx
        push   ebx
        push   eax
        db  0E8h, 05Ch, 0F6h, 0FEh, 0FFh
        add    esp, 10h
        cmp    eax, 1
        ret
WSOCK_Service_02:
        test   eax, eax
        je     SHORT loc_1_0A31
        xor    edx, edx
loc_1_0A31:
        push   edx
        push   eax
        db  0E8h, 0F9h, 0F6h, 0FEh, 0FFh
        add    esp, 8
        ret
WSOCK_Service_03:
        jmp    NEAR PTR sub_1_0445
WSOCK_Service_04:
        jmp    NEAR PTR sub_1_054E
loc_1_0A46:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        db  0A3h, 088h, 022h, 000h, 000h
        xor    eax, eax
        ret
sub_1_0A54:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_0A93
        or     ebx, ebx
        je     SHORT loc_1_0A93
        movzx  ecx, ax
        and    ch, 0fh
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0A97
        add    eax, esi
loc_1_0A93:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_0A97:
        xor    eax, eax
        jmp    SHORT loc_1_0A93
sub_1_0A9B:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_0AD6
        or     ebx, ebx
        je     SHORT loc_1_0AD6
        movzx  ecx, ax
        and    ch, 0fh
        add    ebx, ecx
        add    ebx, 0fffh
        shr    ebx, 0ch
        shr    eax, 0ch
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0ADF
loc_1_0AD6:
        mov    eax, 1
loc_1_0ADB:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_0ADF:
        xor    eax, eax
        jmp    SHORT loc_1_0ADB
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_0B2C:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 14h]
        movzx  eax, word ptr [esp + 18h]
        movzx  esi, word ptr [esp + 1ch]
        or     eax, eax
        je     SHORT loc_1_0B68
        test   dword ptr [ebx], 20h
        je     SHORT loc_1_0B6D
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0B68
        add    eax, esi
        cmp    eax, 110000h
        jae    SHORT loc_1_0B68
loc_1_0B65:
        add    eax, dword ptr [ebx + 4]
loc_1_0B68:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret
loc_1_0B6D:
        shl    eax, 4
        add    eax, esi
        jmp    SHORT loc_1_0B65
loc_1_0B74:
        ret
        db  053h, 057h, 056h, 06Ah, 000h, 068h, 074h, 00Bh, 000h, 000h, 0FFh, 074h, 024h, 024h, 0FFh, 074h
        db  024h, 024h, 0FFh, 074h, 024h, 024h, 0FFh, 074h, 024h, 024h, 0CDh, 020h, 006h, 000h, 017h, 000h
        db  083h, 0C4h, 018h, 05Eh, 05Fh, 05Bh, 0C3h
sub_1_0B9C:
        push   dword ptr [esp + 4]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        ret
sub_1_0BB2:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_0BBD:
        db  0CDh, 020h, 00Dh, 080h, 02Ah, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0BD0:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        sub    eax, 4
        sub    edx, edx
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 8]
        div    esi
        lea    edx, [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        je     SHORT loc_1_0BFE
loc_1_0BF0:
        lea    ecx, [edx + esi]
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        jne    SHORT loc_1_0BF0
loc_1_0BFE:
        sub    edx, esi
        pop    esi
        mov    dword ptr [edx], 0
        ret
sub_1_0C08:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_0C16
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
loc_1_0C16:
        ret
sub_1_0C17:
        mov    eax, dword ptr [esp + 4]
        db  08Bh
loc_1_0C1C:
        dec    esp
        and    al, 8
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax], ecx
        ret
        db  000h, 000h
sub_1_0C28:
        db  0CDh, 020h, 00Dh, 080h, 040h, 000h
sub_1_0C2E:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_0C3B
        db  0CDh, 020h, 00Eh, 080h, 040h, 000h
loc_1_0C3B:
        ret
loc_1_0C3C:
        db  0CDh, 020h, 011h, 080h, 040h, 000h
        add    byte ptr [eax], al
        db  058h, 008h, 000h, 000h, 015h, 00Ah, 000h, 000h, 02Bh, 00Ah, 000h, 000h, 03Ch, 00Ah, 000h, 000h
        db  041h, 00Ah, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 03Eh, 000h, 001h, 000h, 000h, 000h, 057h, 053h, 04Fh, 043h
        db  04Bh, 020h, 020h, 020h, 001h, 060h, 000h, 054h, 05Fh, 008h, 000h, 000h, 069h, 009h, 000h, 000h
        db  069h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  044h, 00Ch, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  0FFh, 0FFh, 0FFh, 0FFh, 040h, 00Dh, 000h, 000h, 010h, 011h, 000h, 000h, 02Bh, 01Ch, 000h, 000h
        db  0E0h, 011h, 000h, 000h, 020h, 013h, 000h, 000h, 0B2h, 013h, 000h, 000h, 030h, 01Dh, 000h, 000h
        db  040h, 014h, 000h, 000h, 0C0h, 014h, 000h, 000h, 010h, 015h, 000h, 000h, 020h, 016h, 000h, 000h
        db  094h, 016h, 000h, 000h, 0CFh, 016h, 000h, 000h, 040h, 019h, 000h, 000h, 085h, 01Fh, 000h, 000h
        db  050h, 01Ah, 000h, 000h, 0C0h, 01Ah, 000h, 000h, 070h, 001h, 000h, 000h, 0A0h, 002h, 000h, 000h
        db  05Dh, 003h, 000h, 000h, 0D7h, 003h, 000h, 000h, 006h, 004h, 000h, 000h, 02Eh, 004h, 000h, 000h
        db  03Ah, 005h, 000h, 000h, 0B0h, 012h, 000h, 000h, 046h, 00Ah, 000h, 000h, 0F0h, 00Eh, 000h, 000h
        db  022h, 010h, 000h, 000h, 001h, 001h, 000h, 001h, 001h, 001h, 001h, 000h, 000h, 002h, 003h, 003h
        db  000h, 002h, 001h, 000h, 000h, 000h, 003h, 001h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0D40:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0ch]
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_0D63
        mov    esi, 273dh
        jmp    NEAR PTR loc_1_0EE0
loc_1_0D63:
        mov    ecx, dword ptr [edi + 4ch]
        cmp    ecx, 0ah
        jne    SHORT loc_1_0D75
        mov    esi, 2742h
        jmp    NEAR PTR loc_1_0EE0
loc_1_0D75:
        cmp    ecx, 3
        je     SHORT loc_1_0D84
        mov    esi, 2726h
        jmp    NEAR PTR loc_1_0EE0
loc_1_0D84:
        or     eax, eax
        je     SHORT loc_1_0DBB
        cmp    dword ptr [edi + 20h], ebp
        jle    SHORT loc_1_0D97
        mov    esi, 271eh
        jmp    NEAR PTR loc_1_0EE0
loc_1_0D97:
        or     eax, eax
        je     SHORT loc_1_0DBB
        or     ebp, ebp
        je     SHORT loc_1_0DBB
        push   ebp
        push   eax
        call   sub_1_0A54
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0DC5
        mov    esi, 271eh
        jmp    NEAR PTR loc_1_0EE0
loc_1_0DBB:
        mov    dword ptr [esp + 14h], 0
        sub    ebp, ebp
loc_1_0DC5:
        lea    eax, [esp + 10h]
        push   dword ptr [ebx + 10h]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        call   dword ptr [eax + 10h]
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_0E36
        cmp    esi, 2733h
        jne    SHORT loc_1_0E24
        test   byte ptr [edi + 41h], 1
        je     SHORT loc_1_0E24
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        mov    eax, esi
        jne    SHORT loc_1_0DFA
        call   sub_1_0BB2
loc_1_0DFA:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [ebx + 18h]
        push   dword ptr [ebx + 14h]
        push   8
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    esi, esi
        and    esi, 0ffff2748h
        add    esi, 0ffffh
loc_1_0E24:
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_1_0A9B
        add    esp, 8
        jmp    NEAR PTR loc_1_0EE0
loc_1_0E36:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 14h], 0
        mov    dword ptr [eax + 70h], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 6ch], 0ffffffffh
        je     SHORT loc_1_0E93
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax + 24h]
        cmp    ebp, edx
        jb     SHORT loc_1_0E7F
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax + 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_1_0E86
loc_1_0E7F:
        mov    dword ptr [ebx + 0ch], 0
loc_1_0E86:
        push   ebp
        push   dword ptr [esp + 18h]
        call   sub_1_0A9B
        add    esp, 8
loc_1_0E93:
        mov    esi, dword ptr [esp + 20h]
        or     esi, esi
        jne    SHORT loc_1_0EA9
        call   sub_1_0BBD
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 64h], eax
        jmp    SHORT loc_1_0EB0
loc_1_0EA9:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 64h], esi
loc_1_0EB0:
        mov    eax, dword ptr [esp + 10h]
        db  0C7h, 000h, 07Ch, 022h, 000h, 000h
        mov    ecx, dword ptr [esp + 10h]
        db  0A1h, 080h, 022h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        sub    esi, esi
        db  08Bh, 00Dh, 080h, 022h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 080h, 022h, 000h, 000h
        mov    dword ptr [ebx + 8], eax
loc_1_0EE0:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_0EF0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 10h], eax
        push   eax
        push   dword ptr [esi]
        call   sub_1_0A54
        add    esp, 8
        mov    ebx, eax
        push   14h
        push   dword ptr [esi + 10h]
        call   sub_1_0A54
        add    esp, 8
        mov    esi, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_1002
        or     esi, esi
        je     NEAR PTR loc_1_1002
        mov    ecx, dword ptr [edi + 4ch]
        cmp    ecx, 0ah
        jne    SHORT loc_1_0F43
        mov    ebp, 2742h
        jmp    NEAR PTR loc_1_0FDB
loc_1_0F43:
        mov    eax, dword ptr [edi + 40h]
        test   al, 80h
        je     SHORT loc_1_0F54
        mov    ebp, 274ah
        jmp    NEAR PTR loc_1_0FDB
loc_1_0F54:
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_0F6B
        cmp    ecx, 2
        je     SHORT loc_1_0F8D
        cmp    ecx, 6
        je     SHORT loc_1_0F8D
        mov    ebp, 2726h
        jmp    SHORT loc_1_0FDB
loc_1_0F6B:
        cmp    ecx, 5
        jne    SHORT loc_1_0F7C
        test   ah, 1
        jne    SHORT loc_1_0F7C
        mov    ebp, 2733h
        jmp    SHORT loc_1_0FDB
loc_1_0F7C:
        cmp    ecx, 6
        je     SHORT loc_1_0F8D
        cmp    ecx, 7
        je     SHORT loc_1_0F8D
        mov    ebp, 2749h
        jmp    SHORT loc_1_0FDB
loc_1_0F8D:
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   0
        push   80000000h
        mov    eax, dword ptr [edi + 1ch]
        push   dword ptr [esp + 20h]
        push   ebx
        push   edi
        call   dword ptr [eax + 1ch]
        add    esp, 1ch
        mov    ebp, eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 4], eax
        cmp    ebp, 2733h
        jne    SHORT loc_1_0FDB
        push   4
        push   esi
        push   dword ptr [esp + 18h]
        push   ebx
        push   1
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    ebp, ebp
        and    ebp, 2747h
        je     SHORT loc_1_1018
loc_1_0FDB:
        or     ebp, ebp
        je     SHORT loc_1_0FE6
        mov    dword ptr [esi + 4], 0
loc_1_0FE6:
        push   dword ptr [esi]
        call   sub_1_092C
        add    esp, 4
        push   dword ptr [esp + 10h]
        push   ebx
        call   sub_1_0A9B
        add    esp, 8
        push   14h
        push   esi
        jmp    SHORT loc_1_1010
loc_1_1002:
        mov    ebp, 2747h
        or     ebx, ebx
        je     SHORT loc_1_1018
        push   dword ptr [esp + 10h]
        push   ebx
loc_1_1010:
        call   sub_1_0A9B
        add    esp, 8
loc_1_1018:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_1022:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        push   14h
        mov    edi, dword ptr [esi + 8]
        push   dword ptr [esi + 10h]
        call   sub_1_0A54
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_104D
        mov    ebp, 2747h
        jmp    NEAR PTR loc_1_10F8
loc_1_104D:
        mov    ecx, dword ptr [edi + 4ch]
        cmp    ecx, 0ah
        jne    SHORT loc_1_105C
        mov    ebp, 2742h
        jmp    SHORT loc_1_10D8
loc_1_105C:
        mov    eax, dword ptr [edi + 40h]
        test   al, 40h
        je     SHORT loc_1_106A
        mov    ebp, 274ah
        jmp    SHORT loc_1_10D8
loc_1_106A:
        cmp    dword ptr [edi + 2ch], 2
        je     SHORT loc_1_108D
        cmp    ecx, 5
        jne    SHORT loc_1_1081
        test   ah, 1
        jne    SHORT loc_1_1081
        mov    ebp, 2733h
        jmp    SHORT loc_1_10D8
loc_1_1081:
        cmp    ecx, 6
        je     SHORT loc_1_108D
        mov    ebp, 2749h
        jmp    SHORT loc_1_10D8
loc_1_108D:
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        call   dword ptr [eax + 20h]
        add    esp, 1ch
        mov    ebp, eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 4], eax
        cmp    ebp, 2733h
        jne    SHORT loc_1_10D8
        push   2
        push   ebx
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi]
        push   2
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    ebp, ebp
        and    ebp, 2747h
        je     SHORT loc_1_10F8
loc_1_10D8:
        or     ebp, ebp
        je     SHORT loc_1_10E3
        mov    dword ptr [ebx + 4], 0
loc_1_10E3:
        push   dword ptr [ebx]
        call   sub_1_092C
        add    esp, 4
        push   14h
        push   ebx
        call   sub_1_0A9B
        add    esp, 8
loc_1_10F8:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1110:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [edi + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_112E
        mov    ebx, 2742h
        jmp    NEAR PTR loc_1_11D0
loc_1_112E:
        cmp    eax, 1
        je     SHORT loc_1_113D
        mov    ebx, 2726h
        jmp    NEAR PTR loc_1_11D0
loc_1_113D:
        movzx  eax, word ptr [ecx]
        cmp    dword ptr [edi + 28h], eax
        je     SHORT loc_1_114F
        mov    ebx, 273fh
        jmp    NEAR PTR loc_1_11D0
loc_1_114F:
        push   dword ptr [esi + 8]
        push   ecx
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        call   dword ptr [eax + 0ch]
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_1185
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_117F
        mov    dword ptr [edi + 54h], 0
        push   0
        push   2
        push   edi
        call   sub_1_0445
        add    esp, 0ch
        jmp    SHORT loc_1_11D0
loc_1_117F:
        and    dword ptr [edi + 54h], 0ffffffefh
        jmp    SHORT loc_1_11D0
loc_1_1185:
        cmp    ebx, 2733h
        jne    SHORT loc_1_11D0
        test   byte ptr [edi + 41h], 1
        je     SHORT loc_1_11D0
        and    dword ptr [edi + 54h], 0ffffffedh
        mov    ebx, dword ptr [esp + 14h]
        or     ebx, ebx
        mov    eax, ebx
        jne    SHORT loc_1_11A6
        call   sub_1_0BB2
loc_1_11A6:
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        push   2
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0ffff2748h
        add    ebx, 0ffffh
loc_1_11D0:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11E0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [edi + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_11FC
        mov    ebx, 2742h
        jmp    NEAR PTR loc_1_12A6
loc_1_11FC:
        cmp    eax, 6
        jne    SHORT loc_1_1211
        cmp    dword ptr [edi + 2ch], 2
        je     SHORT loc_1_1227
        mov    ebx, 2748h
        jmp    NEAR PTR loc_1_12A6
loc_1_1211:
        cmp    eax, 1
        je     SHORT loc_1_121B
        cmp    eax, 2
        jne    SHORT loc_1_1220
loc_1_121B:
        cmp    eax, 5
        jne    SHORT loc_1_1227
loc_1_1220:
        mov    ebx, 2726h
        jmp    SHORT loc_1_12A6
loc_1_1227:
        and    dword ptr [edi + 50h], 0fffffeffh
        push   dword ptr [esi + 8]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        call   dword ptr [eax + 14h]
        add    esp, 0ch
        mov    ebx, eax
        cmp    ebx, 2733h
        jne    SHORT loc_1_128B
        test   byte ptr [edi + 41h], 1
        je     SHORT loc_1_12A6
        mov    ebx, dword ptr [esp + 14h]
        or     ebx, ebx
        mov    eax, ebx
        jne    SHORT loc_1_125C
        call   sub_1_0BB2
loc_1_125C:
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        push   110h
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0ffff2748h
        add    ebx, 0ffffh
        jmp    SHORT loc_1_12A6
loc_1_128B:
        or     ebx, ebx
        jne    SHORT loc_1_12A6
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_1_12A6
        and    dword ptr [edi + 54h], 0fffffffdh
        push   0
        push   2
        push   edi
        call   sub_1_0445
        add    esp, 0ch
loc_1_12A6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h
loc_1_12B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        db  0BFh, 0F0h, 021h, 000h, 000h
        mov    ebx, dword ptr [esi + 10h]
        sub    ebp, ebp
        db  0A1h, 084h, 022h, 000h, 000h
        cmp    eax, ebp
        je     SHORT loc_1_12EE
loc_1_12CB:
        push   dword ptr [edi + 0ch]
        push   ebx
        push   0
        mov    eax, dword ptr [edi + 8]
        push   0
        call   dword ptr [eax]
        add    esp, 10h
        or     eax, eax
        db  0A1h, 084h, 022h, 000h, 000h
        je     SHORT loc_1_12EC
        add    edi, 10h
        inc    ebp
        cmp    eax, ebp
        ja     SHORT loc_1_12CB
loc_1_12EC:
        cmp    eax, ebp
loc_1_12EE:
        jbe    SHORT loc_1_1314
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [eax + 38h]
        or     ecx, ecx
        je     SHORT loc_1_1314
        push   dword ptr [edi + 0ch]
        lea    eax, [esi + 0ch]
        push   eax
        push   dword ptr [esi + 4]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi]
        push   dword ptr [esi + 14h]
        call   ecx
        add    esp, 18h
        jmp    SHORT loc_1_1319
loc_1_1314:
        mov    eax, 273bh
loc_1_1319:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h
loc_1_1320:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebp + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_1346
        mov    eax, 2742h
        jmp    SHORT loc_1_13AA
loc_1_1346:
        cmp    eax, 6
        je     SHORT loc_1_1357
        cmp    eax, 7
        je     SHORT loc_1_1357
        mov    eax, 2749h
        jmp    SHORT loc_1_13AA
loc_1_1357:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [ebp + 24h], eax
        jbe    SHORT loc_1_1367
        mov    eax, 271eh
        jmp    SHORT loc_1_13AA
loc_1_1367:
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_137F
        mov    eax, 271eh
        jmp    SHORT loc_1_13AA
loc_1_137F:
        mov    edx, dword ptr [ebp + 24h]
        mov    edi, eax
        mov    esi, dword ptr [ebp + 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 24h]
        mov    dword ptr [ebx + 8], ecx
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_1_0A9B
        add    esp, 8
        sub    eax, eax
loc_1_13AA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_13B2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebp + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_13D8
        mov    eax, 2742h
        jmp    SHORT loc_1_1437
loc_1_13D8:
        cmp    eax, 1
        jne    SHORT loc_1_13E4
        mov    eax, 2726h
        jmp    SHORT loc_1_1437
loc_1_13E4:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [ebp + 20h], eax
        jbe    SHORT loc_1_13F4
        mov    eax, 271eh
        jmp    SHORT loc_1_1437
loc_1_13F4:
        push   dword ptr [esp + 10h]
        push   ecx
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_140C
        mov    eax, 271eh
        jmp    SHORT loc_1_1437
loc_1_140C:
        mov    edx, dword ptr [ebp + 20h]
        mov    edi, eax
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebx + 8], ecx
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_1_0A9B
        add    esp, 8
        sub    eax, eax
loc_1_1437:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h
loc_1_1440:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 4], eax
        cmp    dword ptr [ecx + 4ch], 0ah
        jne    SHORT loc_1_1461
        mov    eax, 2742h
        jmp    SHORT loc_1_14B6
loc_1_1461:
        sub    eax, eax
        cmp    edx, 8004667eh
        je     SHORT loc_1_147D
        lea    eax, [esp + 4]
        push   eax
        push   edx
        mov    eax, dword ptr [ecx + 1ch]
        push   ecx
        call   dword ptr [eax + 30h]
        add    esp, 0ch
        jmp    SHORT loc_1_14AF
loc_1_147D:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_148D
        and    dword ptr [ecx + 40h], 0fffffeffh
        jmp    SHORT loc_1_14AF
loc_1_148D:
        cmp    dword ptr [ecx + 58h], 0
        je     SHORT loc_1_149A
        mov    eax, 2726h
        jmp    SHORT loc_1_14AF
loc_1_149A:
        mov    dword ptr [ecx + 58h], 0
        mov    dword ptr [ecx + 60h], 0
        or     dword ptr [ecx + 40h], 100h
loc_1_14AF:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [esi + 8], ecx
loc_1_14B6:
        pop    esi
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h
loc_1_14C0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [edx + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_14D8
        mov    eax, 2742h
        jmp    SHORT loc_1_150D
loc_1_14D8:
        cmp    dword ptr [edx + 2ch], 2
        jne    SHORT loc_1_14E5
        mov    eax, 273dh
        jmp    SHORT loc_1_150D
loc_1_14E5:
        cmp    eax, 6
        je     SHORT loc_1_1508
        cmp    eax, 5
        je     SHORT loc_1_1508
        cmp    eax, 2
        je     SHORT loc_1_14FB
        mov    eax, 2726h
        jmp    SHORT loc_1_150D
loc_1_14FB:
        push   ecx
        mov    eax, dword ptr [edx + 1ch]
        push   edx
        call   dword ptr [eax + 18h]
        add    esp, 8
        jmp    SHORT loc_1_150D
loc_1_1508:
        mov    eax, 2748h
loc_1_150D:
        ret
        db  000h, 000h
loc_1_1510:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [edi + 4ch]
        cmp    ecx, 0ah
        jne    SHORT loc_1_1530
        mov    eax, 2742h
        jmp    NEAR PTR loc_1_161A
loc_1_1530:
        mov    eax, dword ptr [edi + 40h]
        test   al, 80h
        je     SHORT loc_1_1541
        mov    eax, 274ah
        jmp    NEAR PTR loc_1_161A
loc_1_1541:
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_156A
        cmp    ecx, 2
        je     SHORT loc_1_155B
        cmp    ecx, 6
        je     SHORT loc_1_155B
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_161A
loc_1_155B:
        test   dl, 1
        je     SHORT loc_1_159F
        mov    eax, 273dh
        jmp    NEAR PTR loc_1_161A
loc_1_156A:
        test   byte ptr [edi + 3ch], 1
        je     SHORT loc_1_157A
        mov    eax, 2746h
        jmp    NEAR PTR loc_1_161A
loc_1_157A:
        cmp    ecx, 5
        jne    SHORT loc_1_158E
        test   ah, 1
        jne    SHORT loc_1_158E
        mov    eax, 2733h
        jmp    NEAR PTR loc_1_161A
loc_1_158E:
        cmp    ecx, 6
        je     SHORT loc_1_159F
        cmp    ecx, 7
        je     SHORT loc_1_159F
        mov    eax, 2749h
        jmp    SHORT loc_1_161A
loc_1_159F:
        lea    ebx, [esi + 18h]
        push   ebx
        lea    eax, [esi + 14h]
        push   eax
        push   dword ptr [esi + 4]
        push   edx
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        call   dword ptr [eax + 1ch]
        add    esp, 1ch
        cmp    eax, 2733h
        jne    SHORT loc_1_1604
        test   byte ptr [edi + 41h], 1
        je     SHORT loc_1_1604
        mov    ebp, dword ptr [esp + 18h]
        or     ebp, ebp
        mov    eax, ebp
        jne    SHORT loc_1_15D6
        call   sub_1_0BB2
loc_1_15D6:
        cmp    ebp, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 20h]
        push   dword ptr [esi + 1ch]
        push   1
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0ffff2748h
        add    eax, 0ffffh
        mov    ecx, dword ptr [edi + 6ch]
        mov    dword ptr [esi + 24h], ecx
loc_1_1604:
        or     eax, eax
        jne    SHORT loc_1_161A
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_161A
        mov    ecx, dword ptr [esi + 0ch]
        cmp    dword ptr [ebx], ecx
        jbe    SHORT loc_1_161A
        mov    eax, 2738h
loc_1_161A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h
loc_1_1620:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   dword ptr [edi]
        push   dword ptr [edi + 0ch]
        call   sub_1_1776
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1657
        push   dword ptr [edi + 4]
        push   dword ptr [edi + 10h]
        call   sub_1_1776
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1691
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 14h]
        call   sub_1_1776
        add    esp, 8
loc_1_1657:
        or     eax, eax
        jne    SHORT loc_1_1691
        mov    esi, dword ptr [esp + 10h]
        or     esi, esi
        mov    eax, esi
        jne    SHORT loc_1_166A
        call   sub_1_0BB2
loc_1_166A:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [edi + 1ch]
        push   dword ptr [edi + 18h]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 14h]
        push   dword ptr [edi + 4]
        push   dword ptr [edi + 10h]
        push   dword ptr [edi]
        push   dword ptr [edi + 0ch]
        call   sub_1_02DD
        add    esp, 28h
loc_1_1691:
        pop    edi
        pop    esi
        ret
loc_1_1694:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   dword ptr [esi]
        push   dword ptr [esi + 0ch]
        call   sub_1_181A
        add    esp, 0ch
        push   edi
        push   dword ptr [esi + 4]
        push   dword ptr [esi + 10h]
        call   sub_1_181A
        add    esp, 0ch
        push   edi
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 14h]
        call   sub_1_181A
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_1_16CF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 088h, 022h, 000h, 000h, 000h
        jne    SHORT loc_1_16E6
        mov    eax, 276dh
        jmp    NEAR PTR loc_1_1771
loc_1_16E6:
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [edi + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_16FE
        mov    eax, 2742h
        jmp    SHORT loc_1_1771
loc_1_16FE:
        or     ebx, ebx
        jne    SHORT loc_1_170A
        sub    ecx, ecx
        sub    ebp, ebp
        sub    edx, edx
        jmp    SHORT loc_1_172D
loc_1_170A:
        mov    ecx, dword ptr [esi + 4]
        mov    ebp, dword ptr [esi + 8]
        mov    edx, dword ptr [edi + 54h]
        cmp    eax, 2
        je     SHORT loc_1_171F
        cmp    eax, 3
        je     SHORT loc_1_172A
        jmp    SHORT loc_1_172D
loc_1_171F:
        cmp    dword ptr [edi + 2ch], 1
        jne    SHORT loc_1_172D
        or     edx, 2
        jmp    SHORT loc_1_172D
loc_1_172A:
        or     edx, 37h
loc_1_172D:
        and    dword ptr [edi + 40h], 0fffffeffh
        mov    dword ptr [edi + 60h], ebx
        mov    dword ptr [edi + 54h], edx
        mov    dword ptr [edi + 5ch], ebp
        not    edx
        mov    dword ptr [edi + 58h], ecx
        mov    eax, dword ptr [edi + 50h]
        and    eax, edx
        and    ebx, eax
        mov    ebp, 1
loc_1_174E:
        test   ebp, ebx
        je     SHORT loc_1_175E
        push   0
        push   ebp
        push   edi
        call   sub_1_05A6
        add    esp, 0ch
loc_1_175E:
        add    ebp, ebp
        test   ebp, 3fh
        jne    SHORT loc_1_174E
        mov    dword ptr [esi + 0ch], 0
        sub    eax, eax
loc_1_1771:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1776:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        jne    SHORT loc_1_1789
        sub    eax, eax
        jmp    NEAR PTR loc_1_1815
loc_1_1789:
        mov    ebx, edi
        imul   ebx, ebx, 0ch
        push   ebx
        push   dword ptr [esp + 1ch]
        call   sub_1_0A54
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_17AA
        mov    eax, 271eh
        jmp    SHORT loc_1_1815
loc_1_17AA:
        sub    esi, esi
        mov    ebp, dword ptr [esp + 18h]
        mov    eax, edi
        dec    edi
        cmp    eax, esi
        je     SHORT loc_1_1806
loc_1_17B7:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     SHORT loc_1_1801
        push   eax
        call   sub_1_18BE
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_1801
        mov    ecx, dword ptr [ebp]
        or     ecx, ecx
        je     SHORT loc_1_1801
        mov    eax, dword ptr [ecx + 4ch]
        cmp    eax, 9
        je     SHORT loc_1_1801
        cmp    eax, 8
        je     SHORT loc_1_1801
        cmp    eax, 0ah
        je     SHORT loc_1_17FA
        test   byte ptr [ebp + 4], 20h
        je     SHORT loc_1_17EE
        and    dword ptr [ecx + 54h], 0ffffffdfh
loc_1_17EE:
        add    ebp, 0ch
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    SHORT loc_1_17B7
        jmp    SHORT loc_1_1806
loc_1_17FA:
        mov    esi, 2742h
        jmp    SHORT loc_1_1806
loc_1_1801:
        mov    esi, 2736h
loc_1_1806:
        push   ebx
        push   dword ptr [esp + 1ch]
        call   sub_1_0A9B
        add    esp, 8
        mov    eax, esi
loc_1_1815:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_181A:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 18h], 0
        je     NEAR PTR loc_1_18B6
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 20h]
        or     eax, eax
        jne    SHORT loc_1_183D
        call   sub_1_0BB2
loc_1_183D:
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 18h]
        dec    dword ptr [esp + 18h]
        or     eax, eax
        je     SHORT loc_1_18B6
loc_1_184D:
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        push   edi
        call   sub_1_18BE
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1864
        add    esi, 0ch
        jmp    SHORT loc_1_18AA
loc_1_1864:
        mov    eax, dword ptr [edi + 54h]
        not    eax
        and    eax, dword ptr [edi + 50h]
        test   ebx, eax
        jne    SHORT loc_1_1876
        mov    dword ptr [esi], 0
loc_1_1876:
        add    esi, 0ch
        mov    ebp, dword ptr [edi + 8]
        mov    eax, edi
        sub    eax, ebp
        cmp    eax, -8
        je     SHORT loc_1_18AA
loc_1_1885:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [eax + 18h], ebx
        jne    SHORT loc_1_18A1
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 20h], ecx
        jne    SHORT loc_1_18A1
        push   eax
        call   sub_1_036E
        add    esp, 4
loc_1_18A1:
        mov    eax, edi
        sub    eax, ebp
        cmp    eax, -8
        jne    SHORT loc_1_1885
loc_1_18AA:
        mov    eax, dword ptr [esp + 18h]
        dec    dword ptr [esp + 18h]
        or     eax, eax
        jne    SHORT loc_1_184D
loc_1_18B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_18BE:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4ch]
        cmp    ecx, 1
        jb     SHORT loc_1_192E
        cmp    ecx, 0ah
        ja     SHORT loc_1_192E
        cmp    ecx, 8
        je     SHORT loc_1_192E
        cmp    ecx, 9
        je     SHORT loc_1_192E
        cmp    ecx, 0ah
        je     SHORT loc_1_192E
        test   dword ptr [eax + 3ch], 0fffffffch
        jne    SHORT loc_1_192E
        test   dword ptr [eax + 40h], 0fffffe00h
        jne    SHORT loc_1_192E
        test   dword ptr [eax + 50h], 0ffffffc0h
        jne    SHORT loc_1_192E
        test   dword ptr [eax + 54h], 0ffffffc0h
        jne    SHORT loc_1_192E
        test   dword ptr [eax + 60h], 0ffffffc0h
        jne    SHORT loc_1_192E
        test   byte ptr [eax + 3], 80h
        je     SHORT loc_1_192E
        test   byte ptr [eax + 7], 80h
        je     SHORT loc_1_192E
        test   byte ptr [eax + 0bh], 80h
        je     SHORT loc_1_192E
        test   byte ptr [eax + 0fh], 80h
        je     SHORT loc_1_192E
        test   byte ptr [eax + 1fh], 80h
        mov    eax, 1
        jne    SHORT loc_1_1930
loc_1_192E:
        sub    eax, eax
loc_1_1930:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1940:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 10h]
        mov    edx, dword ptr [esi + 4]
        mov    ebx, dword ptr [edi + 4ch]
        cmp    ebx, 0ah
        jne    SHORT loc_1_1962
        mov    eax, 2742h
        jmp    NEAR PTR loc_1_1A4B
loc_1_1962:
        mov    eax, dword ptr [edi + 40h]
        test   al, 40h
        je     SHORT loc_1_1973
        mov    eax, 274ah
        jmp    NEAR PTR loc_1_1A4B
loc_1_1973:
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_19BF
        cmp    ebx, 6
        je     SHORT loc_1_198C
        or     edx, edx
        jne    SHORT loc_1_199A
        mov    eax, 2749h
        jmp    NEAR PTR loc_1_1A4B
loc_1_198C:
        or     edx, edx
        je     SHORT loc_1_199A
        mov    eax, 2748h
        jmp    NEAR PTR loc_1_1A4B
loc_1_199A:
        test   cl, 1
        je     SHORT loc_1_19A9
        mov    eax, 273dh
        jmp    NEAR PTR loc_1_1A4B
loc_1_19A9:
        or     edx, edx
        je     SHORT loc_1_19E9
        movzx  eax, word ptr [edx]
        cmp    dword ptr [edi + 28h], eax
        je     SHORT loc_1_19E9
        mov    eax, 273fh
        jmp    NEAR PTR loc_1_1A4B
loc_1_19BF:
        test   byte ptr [edi + 3ch], 1
        je     SHORT loc_1_19CC
        mov    eax, 2746h
        jmp    SHORT loc_1_1A4B
loc_1_19CC:
        cmp    ebx, 5
        jne    SHORT loc_1_19DD
        test   ah, 1
        jne    SHORT loc_1_19DD
        mov    eax, 2733h
        jmp    SHORT loc_1_1A4B
loc_1_19DD:
        cmp    ebx, 6
        je     SHORT loc_1_19E9
        mov    eax, 2749h
        jmp    SHORT loc_1_1A4B
loc_1_19E9:
        lea    eax, [esi + 18h]
        push   eax
        push   dword ptr [esi + 14h]
        push   edx
        push   ecx
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        call   dword ptr [eax + 20h]
        add    esp, 1ch
        cmp    eax, 2733h
        jne    SHORT loc_1_1A4B
        test   byte ptr [edi + 41h], 1
        je     SHORT loc_1_1A4B
        mov    ebx, dword ptr [esp + 14h]
        or     ebx, ebx
        mov    eax, ebx
        jne    SHORT loc_1_1A1D
        call   sub_1_0BB2
loc_1_1A1D:
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 20h]
        push   dword ptr [esi + 1ch]
        push   2
        push   edi
        call   sub_1_01AC
        add    esp, 18h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0ffff2748h
        add    eax, 0ffffh
        mov    ecx, dword ptr [edi + 70h]
        mov    dword ptr [esi + 24h], ecx
loc_1_1A4B:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h
loc_1_1A50:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [edx + 4ch]
        cmp    eax, 0ah
        jne    SHORT loc_1_1A68
        mov    eax, 2742h
        jmp    SHORT loc_1_1AB5
loc_1_1A68:
        cmp    dword ptr [edx + 2ch], 1
        jne    SHORT loc_1_1A7F
        cmp    eax, 6
        je     SHORT loc_1_1A7F
        cmp    eax, 7
        je     SHORT loc_1_1A7F
        mov    eax, 2749h
        jmp    SHORT loc_1_1AB5
loc_1_1A7F:
        or     ecx, ecx
        je     SHORT loc_1_1A94
        cmp    ecx, 1
        je     SHORT loc_1_1A9D
        cmp    ecx, 2
        je     SHORT loc_1_1AA3
        mov    eax, 2726h
        jmp    SHORT loc_1_1AB5
loc_1_1A94:
        or     dword ptr [edx + 40h], 80h
        jmp    SHORT loc_1_1AAA
loc_1_1A9D:
        or     dword ptr [edx + 40h], 40h
        jmp    SHORT loc_1_1AAA
loc_1_1AA3:
        or     dword ptr [edx + 40h], 0c0h
loc_1_1AAA:
        push   ecx
        mov    eax, dword ptr [edx + 1ch]
        push   edx
        call   dword ptr [eax + 34h]
        add    esp, 8
loc_1_1AB5:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1AC0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        db  0BFh, 0F0h, 021h, 000h, 000h
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 0ch], eax
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 10h], eax
        sub    eax, eax
        db  08Bh, 00Dh, 084h, 022h, 000h, 000h
        cmp    ecx, eax
        je     SHORT loc_1_1B1A
loc_1_1AF1:
        push   dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 8]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 20h]
        call   dword ptr [eax]
        add    esp, 10h
        or     eax, eax
        db  08Bh, 00Dh, 084h, 022h, 000h, 000h
        je     SHORT loc_1_1B1A
        add    edi, 10h
        inc    ebx
        cmp    ebx, ecx
        jb     SHORT loc_1_1AF1
loc_1_1B1A:
        cmp    ebx, ecx
        jae    NEAR PTR loc_1_1C24
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 8]
        push   dword ptr [edi + 0ch]
        call   dword ptr [eax + 4]
        add    esp, 10h
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_1B4D
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_1C24
loc_1_1B4D:
        lea    eax, [ebx + 8]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 68h], eax
        mov    dword ptr [ebx + 4ch], 1
        mov    dword ptr [ebx + 40h], 100h
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 28h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ebx + 2ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 30h], eax
        mov    dword ptr [ebx + 14h], 0
        mov    dword ptr [ebx + 20h], 0
        mov    dword ptr [ebx + 18h], 0
        mov    dword ptr [ebx + 24h], 0
        mov    dword ptr [ebx + 3ch], 0
        mov    dword ptr [ebx + 58h], 0
        mov    dword ptr [ebx + 5ch], 0
        mov    dword ptr [ebx + 60h], 0
        mov    dword ptr [ebx + 54h], 3fh
        cmp    dword ptr [esp + 0ch], 2
        jne    SHORT loc_1_1BCB
        mov    dword ptr [ebx + 54h], 3dh
loc_1_1BCB:
        mov    dword ptr [ebx + 50h], 2
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [ebx + 44h], 0
        mov    dword ptr [ebx + 48h], 0
        mov    dword ptr [ebx + 70h], 0ffffffffh
        mov    dword ptr [ebx + 6ch], 0ffffffffh
        mov    eax, dword ptr [esp + 20h]
        or     eax, eax
        jne    SHORT loc_1_1C01
        call   sub_1_0BBD
loc_1_1C01:
        mov    dword ptr [ebx + 64h], eax
        db  0C7h, 003h, 07Ch, 022h, 000h, 000h
        db  0A1h, 080h, 022h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0A1h, 080h, 022h, 000h, 000h
        mov    dword ptr [eax], ebx
        sub    eax, eax
        db  089h, 01Dh, 080h, 022h, 000h, 000h
        mov    dword ptr [esi + 0ch], ebx
loc_1_1C24:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_1C2B:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax]
        test   byte ptr [esi + 41h], 1
        jne    SHORT loc_1_1C4D
        cmp    word ptr [esi + 10h], 0
        je     SHORT loc_1_1C4D
        cmp    word ptr [esi + 12h], 0
        je     SHORT loc_1_1C4D
        mov    eax, 2733h
        jmp    SHORT loc_1_1C71
loc_1_1C4D:
        cmp    dword ptr [esi + 4ch], 9
        mov    eax, 0
        je     SHORT loc_1_1C71
        push   0fffeh
        push   esi
        call   sub_1_054E
        add    esp, 8
        push   0
        push   esi
        call   sub_1_1CB8
        add    esp, 8
loc_1_1C71:
        pop    esi
        ret
sub_1_1C73:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 07Ch, 022h, 000h, 000h
        db  081h, 0FFh, 07Ch, 022h, 000h, 000h
        je     SHORT loc_1_1CB3
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, 1
        sub    ebp, ebp
loc_1_1C90:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [eax + 64h], esi
        jne    SHORT loc_1_1CAB
        mov    word ptr [eax + 10h], bx
        mov    word ptr [eax + 12h], bp
        push   ebx
        push   eax
        call   sub_1_1CB8
        add    esp, 8
loc_1_1CAB:
        db  081h, 0FFh, 07Ch, 022h, 000h, 000h
        jne    SHORT loc_1_1C90
loc_1_1CB3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1CB8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8]
        lea    edi, [esi + 8]
        sub    eax, esi
        cmp    eax, 8
        je     SHORT loc_1_1CF7
        mov    ebx, dword ptr [esp + 14h]
loc_1_1CD0:
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        or     dword ptr [edx + 14h], 8
        or     ebx, ebx
        je     SHORT loc_1_1CEE
        push   edx
        call   sub_1_036E
        add    esp, 4
loc_1_1CEE:
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 8
        jne    SHORT loc_1_1CD0
loc_1_1CF7:
        mov    dword ptr [esi + 60h], 0
        mov    dword ptr [esi + 4ch], 9
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 1ch]
        push   esi
        call   dword ptr [eax + 8]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1D30:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        mov    esi, dword ptr [eax + 4]
        mov    ebx, dword ptr [eax]
        add    eax, 10h
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esp + 28h]
        cmp    dword ptr [esi + 4ch], 0ah
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 28h]
        mov    edi, dword ptr [eax + 0ch]
        jne    SHORT loc_1_1D72
        mov    dword ptr [esp + 10h], 2742h
        jmp    NEAR PTR loc_1_1F79
loc_1_1D72:
        push   edi
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_212C
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1D91
        mov    dword ptr [esp + 10h], 273ah
        jmp    NEAR PTR loc_1_1F79
loc_1_1D91:
        push   dword ptr [esp + 20h]
        push   ebx
        call   sub_1_0A54
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1DB3
        mov    dword ptr [esp + 10h], 271eh
        jmp    NEAR PTR loc_1_1F79
loc_1_1DB3:
        mov    dword ptr [esp + 10h], 0
        sub    ebx, ebx
        sub    ebp, ebp
        cmp    dword ptr [esp + 1ch], 0ffffh
        je     SHORT loc_1_1DE9
        push   dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 1ch]
        push   dword ptr [esp + 18h]
        push   edi
        push   dword ptr [esp + 28h]
        push   esi
        call   dword ptr [eax + 28h]
        mov    dword ptr [esp + 24h], eax
        add    esp, 14h
        jmp    NEAR PTR loc_1_1F4E
loc_1_1DE9:
        cmp    edi, 1001h
        ja     SHORT loc_1_1E4B
        je     NEAR PTR loc_1_1F23
        cmp    edi, 10h
        ja     SHORT loc_1_1E28
        je     NEAR PTR loc_1_1EE6
        cmp    edi, 1
        je     NEAR PTR loc_1_1EC3
        cmp    edi, 2
        je     NEAR PTR loc_1_1ECB
        cmp    edi, 4
        je     NEAR PTR loc_1_1ED6
        cmp    edi, 8
        je     NEAR PTR loc_1_1EDE
        jmp    SHORT loc_1_1EA3
loc_1_1E28:
        cmp    edi, 20h
        je     NEAR PTR loc_1_1EEE
        cmp    edi, 80h
        je     NEAR PTR loc_1_1EF6
        cmp    edi, 100h
        je     NEAR PTR loc_1_1F1B
        jmp    SHORT loc_1_1EA3
loc_1_1E4B:
        cmp    edi, 1007h
        ja     SHORT loc_1_1E7F
        je     NEAR PTR loc_1_1F37
        cmp    edi, 1002h
        je     NEAR PTR loc_1_1F28
        cmp    edi, 1005h
        je     NEAR PTR loc_1_1F2D
        cmp    edi, 1006h
        je     NEAR PTR loc_1_1F32
        jmp    SHORT loc_1_1EA3
loc_1_1E7F:
        cmp    edi, 1008h
        je     NEAR PTR loc_1_1F3B
        cmp    edi, 0ff7fh
        je     NEAR PTR loc_1_1F40
        cmp    edi, 0ffffff7fh
        je     NEAR PTR loc_1_1F40
loc_1_1EA3:
        push   dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 1ch]
        push   dword ptr [esp + 18h]
        push   edi
        push   dword ptr [esp + 28h]
        push   esi
        call   dword ptr [eax + 28h]
        mov    dword ptr [esp + 24h], eax
        add    esp, 14h
        jmp    NEAR PTR loc_1_1F4E
loc_1_1EC3:
        mov    ebx, dword ptr [esi + 40h]
        and    ebx, 2
        jmp    SHORT loc_1_1F49
loc_1_1ECB:
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, 3
        cmp    eax, 1
        jmp    SHORT loc_1_1F45
loc_1_1ED6:
        mov    ebx, dword ptr [esi + 40h]
        and    ebx, 20h
        jmp    SHORT loc_1_1F49
loc_1_1EDE:
        mov    ebx, dword ptr [esi + 40h]
        and    ebx, 8
        jmp    SHORT loc_1_1F49
loc_1_1EE6:
        mov    ebx, dword ptr [esi + 40h]
        and    ebx, 4
        jmp    SHORT loc_1_1F49
loc_1_1EEE:
        mov    ebx, dword ptr [esi + 40h]
        and    ebx, 1
        jmp    SHORT loc_1_1F49
loc_1_1EF6:
        cmp    dword ptr [esp + 20h], 4
        jae    SHORT loc_1_1F07
        mov    dword ptr [esp + 10h], 271eh
        jmp    SHORT loc_1_1F4E
loc_1_1F07:
        add    esi, 10h
        mov    edi, dword ptr [esp + 14h]
        movsd
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], 4
        jmp    SHORT loc_1_1F4E
loc_1_1F1B:
        mov    ebx, dword ptr [esi + 40h]
        and    ebx, 10h
        jmp    SHORT loc_1_1F49
loc_1_1F23:
        mov    ebx, dword ptr [esi + 34h]
        jmp    SHORT loc_1_1F49
loc_1_1F28:
        mov    ebx, dword ptr [esi + 38h]
        jmp    SHORT loc_1_1F49
loc_1_1F2D:
        mov    ebx, dword ptr [esi + 70h]
        jmp    SHORT loc_1_1F49
loc_1_1F32:
        mov    ebx, dword ptr [esi + 6ch]
        jmp    SHORT loc_1_1F49
loc_1_1F37:
        sub    ebx, ebx
        jmp    SHORT loc_1_1F49
loc_1_1F3B:
        mov    ebx, dword ptr [esi + 2ch]
        jmp    SHORT loc_1_1F49
loc_1_1F40:
        cmp    word ptr [esi + 10h], 1
loc_1_1F45:
        sbb    ebx, ebx
        neg    ebx
loc_1_1F49:
        mov    ebp, 1
loc_1_1F4E:
        or     ebp, ebp
        je     SHORT loc_1_1F69
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax + 14h], ebx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], 0
loc_1_1F69:
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 18h]
        call   sub_1_0A9B
        add    esp, 8
loc_1_1F79:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_1F85:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 20h]
        mov    ebx, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [ecx + 0ch]
        mov    edi, dword ptr [ecx + 14h]
        cmp    dword ptr [ebx + 4ch], 0ah
        jne    SHORT loc_1_1FB9
        mov    esi, 2742h
        jmp    NEAR PTR loc_1_2122
loc_1_1FB9:
        push   ebp
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_212C
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1FD5
        mov    esi, 273ah
        jmp    NEAR PTR loc_1_2122
loc_1_1FD5:
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_0A54
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1FF4
        mov    esi, 271eh
        jmp    NEAR PTR loc_1_2122
loc_1_1FF4:
        cmp    dword ptr [esp + 18h], 0ffffh
        jne    NEAR PTR loc_1_20F9
        cmp    ebp, 80h
        ja     SHORT loc_1_2032
        je     NEAR PTR loc_1_20C3
        cmp    ebp, 1
        je     SHORT loc_1_2073
        cmp    ebp, 4
        je     SHORT loc_1_2083
        cmp    ebp, 8
        je     SHORT loc_1_2093
        cmp    ebp, 10h
        je     SHORT loc_1_20A3
        cmp    ebp, 20h
        je     NEAR PTR loc_1_20B3
        jmp    NEAR PTR loc_1_20F9
loc_1_2032:
        cmp    ebp, 100h
        je     NEAR PTR loc_1_20D4
        cmp    ebp, 1005h
        je     NEAR PTR loc_1_20E4
        cmp    ebp, 1006h
        je     NEAR PTR loc_1_20E9
        cmp    ebp, 0ff7fh
        je     NEAR PTR loc_1_20EE
        cmp    ebp, 0ffffff7fh
        je     NEAR PTR loc_1_20EE
        jmp    NEAR PTR loc_1_20F9
loc_1_2073:
        or     edi, edi
        je     SHORT loc_1_207D
        or     dword ptr [ebx + 40h], 2
        jmp    SHORT loc_1_20F9
loc_1_207D:
        and    dword ptr [ebx + 40h], 0fffffffdh
        jmp    SHORT loc_1_20F9
loc_1_2083:
        or     edi, edi
        je     SHORT loc_1_208D
        or     dword ptr [ebx + 40h], 20h
        jmp    SHORT loc_1_20F9
loc_1_208D:
        and    dword ptr [ebx + 40h], 0ffffffdfh
        jmp    SHORT loc_1_20F9
loc_1_2093:
        or     edi, edi
        je     SHORT loc_1_209D
        or     dword ptr [ebx + 40h], 8
        jmp    SHORT loc_1_20F9
loc_1_209D:
        and    dword ptr [ebx + 40h], 0fffffff7h
        jmp    SHORT loc_1_20F9
loc_1_20A3:
        or     edi, edi
        je     SHORT loc_1_20AD
        or     dword ptr [ebx + 40h], 4
        jmp    SHORT loc_1_20F9
loc_1_20AD:
        and    dword ptr [ebx + 40h], 0fffffffbh
        jmp    SHORT loc_1_20F9
loc_1_20B3:
        or     edi, edi
        je     SHORT loc_1_20BD
        or     dword ptr [ebx + 40h], 1
        jmp    SHORT loc_1_20F9
loc_1_20BD:
        and    dword ptr [ebx + 40h], 0fffffffeh
        jmp    SHORT loc_1_20F9
loc_1_20C3:
        cmp    dword ptr [esp + 14h], 4
        jb     SHORT loc_1_20F9
        lea    edi, [ebx + 10h]
        mov    esi, dword ptr [esp + 10h]
        movsd
        jmp    SHORT loc_1_20F9
loc_1_20D4:
        or     edi, edi
        je     SHORT loc_1_20DE
        or     dword ptr [ebx + 40h], 10h
        jmp    SHORT loc_1_20F9
loc_1_20DE:
        and    dword ptr [ebx + 40h], 0ffffffefh
        jmp    SHORT loc_1_20F9
loc_1_20E4:
        mov    dword ptr [ebx + 70h], edi
        jmp    SHORT loc_1_20F9
loc_1_20E9:
        mov    dword ptr [ebx + 6ch], edi
        jmp    SHORT loc_1_20F9
loc_1_20EE:
        cmp    edi, 1
        sbb    eax, eax
        neg    eax
        mov    word ptr [ebx + 10h], ax
loc_1_20F9:
        push   dword ptr [esp + 14h]
        mov    eax, dword ptr [ebx + 1ch]
        push   dword ptr [esp + 14h]
        push   ebp
        push   dword ptr [esp + 24h]
        push   ebx
        call   dword ptr [eax + 2ch]
        add    esp, 14h
        mov    esi, eax
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_1_0A9B
        add    esp, 8
loc_1_2122:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_212C:
        cmp    dword ptr [esp + 8], 0ffffh
        je     SHORT loc_1_213D
        mov    eax, 1
        jmp    SHORT loc_1_21A6
loc_1_213D:
        mov    eax, dword ptr [esp + 0ch]
        cmp    eax, 100h
        ja     SHORT loc_1_2160
        je     SHORT loc_1_2183
        cmp    eax, 2
        je     SHORT loc_1_2183
        cmp    eax, 8
        je     SHORT loc_1_2183
        cmp    eax, 20h
        je     SHORT loc_1_2195
        cmp    eax, 80h
        jmp    SHORT loc_1_217A
loc_1_2160:
        cmp    eax, 1002h
        je     SHORT loc_1_217C
        cmp    eax, 1008h
        je     SHORT loc_1_217C
        cmp    eax, 0ff7fh
        je     SHORT loc_1_2183
        cmp    eax, 0ffffff7fh
loc_1_217A:
        je     SHORT loc_1_2183
loc_1_217C:
        mov    eax, 1
        jmp    SHORT loc_1_21A6
loc_1_2183:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 2ch]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_1_21A6
loc_1_2195:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 2ch]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_21A6:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_21B0:
        push   ebx
        db  079h, 073h
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        jb     SHORT loc_1_2221
        outsb
        db  074h, 043h
        outsd
        outsb
        je     SHORT loc_1_2235
        outsd
        insb
        push   ebx
        je     NEAR PTR loc_1_2225
        push   ebx
        jb     NEAR PTR loc_1_2243
        imul   esp, dword ptr [ebx + 65h], 78565c73h
        inc    esp
        pop    esp
        push   edi
        imul   ebp, dword ptr [esi + 73h], 6b636fh
        add    byte ptr [eax], al
loc_1_21E0:
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2221:
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
loc_1_2235:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2243:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
