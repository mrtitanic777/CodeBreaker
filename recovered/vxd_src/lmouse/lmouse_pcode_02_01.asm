; lmouse_pcode_02_01.asm -- LMOUSE object 2 segment _PTEXT  (part 1/3)
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0E8h, 0A5h, 041h, 000h, 000h, 0E8h, 01Dh, 047h, 000h, 000h, 0E8h, 005h, 058h, 000h, 000h, 0E8h
        db  000h, 089h, 000h, 000h, 0FFh, 035h, 00Ch, 000h, 000h, 000h, 0E8h, 000h, 000h, 000h, 000h, 083h
        db  0C4h, 004h, 0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_2_0028:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], 1
        call   sub_2_45E2
        or     eax, eax
        je     SHORT loc_2_0080
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        je     SHORT loc_2_0066
        push   dword ptr [esi + 8]
        call   sub_2_4619
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_0072
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 8]
        call   sub_2_4399
        add    esp, 8
        jmp    SHORT loc_2_0072
loc_2_0066:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_43B7
        add    esp, 4
loc_2_0072:
        call   sub_2_45E2
        or     eax, eax
        jne    SHORT loc_2_0080
        call   0
loc_2_0080:
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0088:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        or     edx, edx
        je     SHORT loc_2_00C4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        or     word ptr [edx], 4
        mov    eax, dword ptr [ecx + 28h]
        mov    word ptr [edx + 6], ax
        db  0C7h, 042h, 01Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + 18h], 0
        mov    eax, dword ptr [ecx + 44h]
        mov    word ptr [edx + 2], ax
        mov    al, byte ptr [ecx + 5ch]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 38h]
        mov    byte ptr [edx + 5], al
loc_2_00C4:
        mov    eax, 1
        pop    ebp
        ret
loc_2_00CB:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, 2
        je     SHORT loc_2_00E7
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 28h]
        sub    eax, ecx
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_00E7:
        pop    ebp
        ret
sub_2_00E9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  0BEh, 010h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  03Bh, 005h, 010h, 000h, 000h, 000h
        je     SHORT loc_2_010D
loc_2_0101:
        cmp    dword ptr [esi], 0
        je     SHORT loc_2_0185
        add    esi, 8
        cmp    dword ptr [esi], eax
        jne    SHORT loc_2_0101
loc_2_010D:
        cmp    dword ptr [esi], 0
        je     SHORT loc_2_0185
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0185
        call   sub_2_43E7
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_0185
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [edi], eax
        push   edi
        call   dword ptr [esi + 4]
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0150
        push   edi
        call   sub_2_44A3
        add    esp, 4
        sub    edi, edi
        jmp    SHORT loc_2_0185
loc_2_0150:
        push   dword ptr [edi + 38h]
        push   dword ptr [edi + 5ch]
        push   dword ptr [edi + 4ch]
        push   dword ptr [edi + 58h]
        call   sub_2_05C9
        add    esp, 10h
        test   byte ptr [ebx + 8], 10h
        je     SHORT loc_2_0185
        push   dword ptr [edi + 44h]
        push   dword ptr [edi + 5ch]
        mov    eax, dword ptr [ebx + 14h]
        push   dword ptr [edi + 38h]
        push   dword ptr [eax + 0ch]
        call   sub_2_5A0F
        add    esp, 10h
        or     dword ptr [ebx + 8], 2
loc_2_0185:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_018C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    ebx, ebx
        mov    edi, dword ptr [esi]
        test   byte ptr [esi + 8], 1
        je     SHORT loc_2_01C6
        push   esi
        push   dword ptr [esi + 0ch]
        call   sub_2_00E9
        add    esp, 8
        cmp    eax, ebx
        je     SHORT loc_2_01C2
        push   dword ptr [esi + 14h]
        push   eax
        call   sub_2_0088
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_2_01FF
loc_2_01C2:
        sub    eax, eax
        jmp    SHORT loc_2_01FF
loc_2_01C6:
        cmp    dword ptr [edi], 0
        jne    SHORT loc_2_01CF
        sub    eax, eax
        jmp    SHORT loc_2_01FF
loc_2_01CF:
        push   esi
        push   dword ptr [edi]
        call   sub_2_00E9
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_01E1
        inc    ebx
        jmp    SHORT loc_2_01E4
loc_2_01E1:
        add    edi, 4
loc_2_01E4:
        cmp    dword ptr [edi], 0
        je     SHORT loc_2_01FD
        call   sub_2_45E2
        cmp    eax, 6
        jae    SHORT loc_2_01FD
        test   byte ptr [esi + 8], 2
        je     SHORT loc_2_01CF
        or     ebx, ebx
        je     SHORT loc_2_01CF
loc_2_01FD:
        mov    eax, ebx
loc_2_01FF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_0204:
        push   ebp
        mov    ebp, esp
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   1
        call   sub_2_02FE
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0222:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   0
        call   sub_2_02FE
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0236:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 54h
        jne    SHORT loc_2_0259
        push   1
        call   sub_2_92D2
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_2_9885
        add    esp, 4
loc_2_0259:
        mov    eax, 1
        pop    ebp
        ret
sub_2_0260:
        push   esi
        sub    esi, esi
        call   sub_2_0791
        cmp    ax, 400h
        jb     NEAR PTR loc_2_02FA
        call   sub_2_8750
        call   sub_2_82CF
        cmp    eax, 62ah
        jne    SHORT loc_2_028F
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_2_02AE
loc_2_028F:
        or     eax, eax
        jne    SHORT loc_2_02A4
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        call   sub_2_9040
        jmp    SHORT loc_2_02AE
loc_2_02A4:
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
loc_2_02AE:
        push   62ah
        call   sub_2_821C
        add    esp, 4
        call   sub_2_4754
        call   sub_2_4180
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02D3:
        add    esp, 0ch
        db  0A3h, 00Ch, 000h, 000h, 000h
        push   0
        call   sub_2_92D2
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_2_9885
        add    esp, 4
        mov    esi, 1
loc_2_02FA:
        mov    eax, esi
        pop    esi
        ret
sub_2_02FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    edi, 1
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, edi
        jne    SHORT loc_2_0323
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_0371
loc_2_0323:
        call   sub_2_45E2
        or     eax, eax
        jne    SHORT loc_2_0333
        call   sub_2_0260
        mov    edi, eax
loc_2_0333:
        call   sub_2_5F08
        mov    ebx, eax
        or     edi, edi
        je     SHORT loc_2_0371
        push   dword ptr [ebp + 0ch]
        push   esi
        push   ebx
        call   sub_2_5E31
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_0363
        push   ebx
        call   sub_2_018C
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_0363
        mov    dword ptr [ebp - 4], 1
loc_2_0363:
        call   sub_2_45E2
        or     eax, eax
        jne    SHORT loc_2_0371
        call   0
loc_2_0371:
        cmp    esi, 1
        mov    eax, edi
        je     SHORT loc_2_037B
        mov    eax, dword ptr [ebp - 4]
loc_2_037B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0382:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, 1
        ret
sub_2_0392:
        push   0
        call   sub_2_0028
        add    esp, 4
        mov    eax, 1
        ret
sub_2_03A2:
        call   sub_2_4787
        push   0
        call   sub_2_92D2
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        call   sub_2_9885
        add    esp, 4
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
sub_2_03D1:
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_03D6:
        test   byte ptr [eax], 40h
        jne    SHORT loc_2_03E9
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jne    SHORT loc_2_03EB
loc_2_03E9:
        sub    eax, eax
loc_2_03EB:
        ret
loc_2_03EC:
        push   esi
        call   sub_2_0028
        add    esp, 4
        cmp    eax, 1
        ret
loc_2_03F9:
        push   esi
        call   sub_2_0028
        add    esp, 4
        cmp    eax, 1
        ret
LMOUSE_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        jmp    NEAR PTR loc_2_5F54
loc_2_040F:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   ebx
        push   ecx
        call   sub_2_0222
        add    esp, 8
        cmp    eax, 1
        ret
loc_2_0423:
        xor    ecx, ecx
        push   ebx
        push   ecx
        call   sub_2_0204
        add    esp, 8
        cmp    eax, 1
        ret
        db  000h
sub_2_0434:
        enter  0, 0
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0
        je     SHORT loc_2_044F
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        jmp    SHORT loc_2_0455
loc_2_044F:
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_2_0455:
        pop    ebx
        leave
        ret
sub_2_0458:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        leave
        ret
sub_2_0467:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        leave
        ret
sub_2_0476:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        xor    ebx, ebx
        db  0CDh, 020h, 010h, 000h, 003h, 000h
        sbb    eax, eax
        leave
        ret
sub_2_0488:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        leave
        ret
        db  0C8h, 000h, 000h, 000h, 08Bh, 045h, 008h, 06Ah, 000h, 050h, 0CDh, 020h, 015h, 001h, 001h, 000h
        db  083h, 0C4h, 008h, 0C9h, 0C3h, 0CDh, 020h, 000h, 000h, 00Dh, 000h, 0C3h
sub_2_04B3:
        push   eax
        push   ecx
        mov    ecx, 10000h
loc_2_04BA:
        in     al, 64h
        test   al, 2
        loopne loc_2_04BA
        pop    ecx
        pop    eax
        ret
sub_2_04C3:
        call   sub_2_04B3
        out    64h, al
        ret
sub_2_04CB:
        push   eax
        mov    al, 0aeh
        call   sub_2_04C3
        pop    eax
        ret
sub_2_04D5:
        push   eax
        mov    al, 0adh
        call   sub_2_04C3
        pop    eax
        ret
sub_2_04DF:
        mov    edx, 60h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 64h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        call   sub_2_04D5
        ret
sub_2_04FB:
        mov    edx, 60h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 64h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        call   sub_2_04CB
        ret
sub_2_0517:
        pushad
        pushfd
        db  0CDh, 020h, 000h, 000h, 00Dh, 000h
        cmp    ax, 20ah
        jb     SHORT loc_2_052D
        db  0CDh, 020h, 010h, 000h, 00Dh, 000h
        jmp    SHORT loc_2_0532
loc_2_052D:
        call   sub_2_04DF
loc_2_0532:
        popfd
        popad
        ret
sub_2_0535:
        pushad
        pushfd
        db  0CDh, 020h, 000h, 000h, 00Dh, 000h
        cmp    ax, 20ah
        jb     SHORT loc_2_054B
        db  0CDh, 020h, 00Fh, 000h, 00Dh, 000h
        jmp    SHORT loc_2_0550
loc_2_054B:
        call   sub_2_04FB
loc_2_0550:
        popfd
        popad
        ret
sub_2_0553:
        push   edi
        push   1
        push   6ch
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0573
        mov    edi, eax
        db  0A3h, 030h, 000h, 000h, 000h
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
loc_2_0573:
        ret
sub_2_0574:
        push   esi
        db  0A1h, 030h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_059D
        mov    esi, eax
        db  0C7h, 005h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    esi
loc_2_059D:
        ret
sub_2_059E:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        ret
sub_2_05A5:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        ret
sub_2_05AC:
        enter  0, 0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 018h, 001h, 001h, 000h
        xor    eax, eax
        jb     SHORT loc_2_05C6
        mov    eax, 1
loc_2_05C6:
        pop    esi
        leave
        ret
sub_2_05C9:
        enter  0, 0
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ah, cl
        mov    ebx, 3
        mov    edx, dword ptr [ebp + 14h]
        mov    dh, bl
        xor    ebx, ebx
        xor    ecx, ecx
        db  0CDh, 020h, 006h, 000h, 00Ch, 000h
        pop    ebx
        leave
        ret
sub_2_05ED:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        cmp    al, 2
        jne    SHORT loc_2_05FA
        mov    al, 82h
loc_2_05FA:
        leave
        ret
        db  0C8h, 000h, 000h, 000h, 053h, 033h, 0DBh, 08Bh, 045h, 008h, 083h, 0F8h, 002h, 074h, 008h, 0CDh
        db  020h, 001h, 000h, 001h, 000h, 0EBh, 006h, 0CDh, 020h, 003h, 000h, 001h, 000h, 08Bh, 0C3h, 05Bh
        db  0C9h, 0C3h, 0C8h, 000h, 000h, 000h, 08Bh, 045h, 008h, 06Ah, 000h, 050h, 0CDh, 020h, 0A7h, 000h
        db  001h, 000h, 083h, 0C4h, 008h, 0C9h, 0C3h
sub_2_0633:
        enter  0, 0
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        mov    eax, 1
        jae    SHORT loc_2_0650
        mov    eax, 0
loc_2_0650:
        pop    esi
        leave
        ret
sub_2_0653:
        enter  0, 0
        push   esi
        mov    edx, dword ptr [ebp + 8]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        sbb    eax, eax
        not    eax
        pop    esi
        leave
        ret
sub_2_0668:
        enter  0, 0
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 0
        je     SHORT loc_2_0683
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        jmp    SHORT loc_2_0689
loc_2_0683:
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_2_0689:
        pop    ebx
        leave
        ret
sub_2_068C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [esi], eax
        sbb    eax, eax
        not    eax
        pop    esi
        leave
        ret
sub_2_06A8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        xor    eax, eax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_2_06CB
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [esi], eax
        sbb    eax, eax
        not    eax
loc_2_06CB:
        pop    esi
        leave
        ret
sub_2_06CE:
        enter  0, 0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        xor    eax, eax
        jb     SHORT loc_2_06E8
        mov    eax, 1
loc_2_06E8:
        pop    esi
        leave
        ret
sub_2_06EB:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax], 0
        jne    SHORT loc_2_0707
        mov    word ptr [eax], cx
        mov    dword ptr [eax + 2], edx
loc_2_0707:
        leave
        ret
sub_2_0709:
        enter  0, 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [eax]
        mov    edx, dword ptr [eax + 2]
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        leave
        ret
sub_2_0721:
        enter  0, 0
        push   edi
        push   1
        push   20h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_078E
        mov    edi, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 4], eax
        mov    al, byte ptr [ebp + 8]
        xor    ah, ah
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 2
        cmp    word ptr [ebp + 10h], 0
        je     SHORT loc_2_0774
        mov    word ptr [edi + 2], 1
        cmp    word ptr [ebp + 10h], 1
        je     SHORT loc_2_0774
        mov    word ptr [edi + 2], 2
        cmp    word ptr [ebp + 10h], 2
        je     SHORT loc_2_0774
        mov    word ptr [edi + 2], 4
loc_2_0774:
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pushfd
        push   eax
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    eax
        popfd
        jae    SHORT loc_2_078E
        xor    eax, eax
loc_2_078E:
        pop    edi
        leave
        ret
sub_2_0791:
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        ret
sub_2_0798:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        leave
        ret
sub_2_07AB:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        ret
        db  000h, 000h
sub_2_07B4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    eax, [eax + 0afh]
        mov    esi, 0ffffffffh
        xchg   dword ptr [eax + 8], esi
        or     esi, esi
        je     SHORT loc_2_07D8
        xor    esi, esi
        xchg   dword ptr [eax + 0ch], esi
        db  0CDh, 020h, 012h, 000h, 001h, 000h
loc_2_07D8:
        pop    esi
        leave
        ret
sub_2_07DB:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    eax, [eax + 0afh]
        xor    esi, esi
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0ch], esi
        pop    esi
        leave
        ret
sub_2_07F3:
        db  0BFh, 098h, 013h, 000h, 000h
loc_2_07F8:
        cmp    dword ptr [edi + 8], eax
        je     SHORT loc_2_080B
        add    edi, 0ch
        db  081h, 0FFh, 0E0h, 013h, 000h, 000h
        jb     SHORT loc_2_07F8
        stc
        jmp    SHORT loc_2_080C
loc_2_080B:
        clc
loc_2_080C:
        ret
sub_2_080D:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    eax, dword ptr [ebp + 8]
        call   sub_2_07F3
        mov    eax, 0
        jb     SHORT loc_2_082C
        mov    dword ptr [edi + 8], 0
        mov    eax, 1
loc_2_082C:
        pop    edi
        leave
        ret
sub_2_082F:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    eax, dword ptr [ebp + 8]
        call   sub_2_07F3
        jb     SHORT loc_2_0844
        mov    eax, 0
        jmp    SHORT loc_2_0863
loc_2_0844:
        mov    eax, 0
        call   sub_2_07F3
        mov    eax, 0
        jb     SHORT loc_2_0863
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [edi]
loc_2_0863:
        pop    edi
        leave
        ret
        db  000h, 000h
loc_2_0868:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 8]
        call   sub_2_0488
        add    esp, 4
        push   esi
        call   sub_2_07B4
        add    esp, 4
        push   dword ptr [esi + 38h]
        call   sub_2_585C
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_1E74
        add    esp, 8
        lea    eax, [esi + 0afh]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_2_08E1:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 0ch], 0
        mov    eax, 0
        jne    SHORT loc_2_08FD
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_2_0A9E
        add    esp, 8
loc_2_08FD:
        pop    ebp
        ret
loc_2_08FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 28h]
        push   edi
        call   sub_2_3F1C
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_2_0927
        sub    eax, eax
        jmp    NEAR PTR loc_2_09CE
loc_2_0927:
        push   edi
        call   sub_2_3F69
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_093E
        mov    eax, 1
        jmp    NEAR PTR loc_2_09CE
loc_2_093E:
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        mov    ax, word ptr [esi + 3ch]
        mov    word ptr [ebp - 2], ax
        push   4b0h
        push   esi
        call   sub_2_0A9E
        add    esp, 8
        cmp    ebx, 1
        mov    eax, dword ptr [esi + 0d7h]
        jne    SHORT loc_2_0983
        add    eax, 2bh
        push   eax
        push   edi
        lea    eax, [ebp - 84h]
        push   eax
        call   sub_2_1258
        jmp    SHORT loc_2_0994
loc_2_0983:
        add    eax, 2bh
        push   eax
        push   edi
        lea    eax, [ebp - 84h]
        push   eax
        call   sub_2_1298
loc_2_0994:
        add    esp, 0ch
        mov    edi, eax
        push   esi
        call   sub_2_07DB
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 13h]
        push   dword ptr [esi + 0cfh]
        lea    ecx, [eax + 2bh]
        push   ecx
        push   eax
        call   sub_2_1FE0
        add    esp, 10h
        movzx  eax, word ptr [ebp - 2]
        push   eax
        push   esi
        call   sub_2_0A9E
        add    esp, 8
        mov    eax, edi
loc_2_09CE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_09D5:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    word ptr [edi], 0
        add    edi, 2
        mov    ecx, 20h
        rep stosd
        pop    edi
        pop    ebp
        ret
loc_2_09F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0d7h]
        lea    ebx, [eax + 2bh]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        lea    eax, [ebp - 84h]
        push   eax
        call   sub_2_09D5
        add    esp, 4
        push   4b0h
        push   ebx
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0A48
        push   8
        push   ebx
        call   sub_2_1F5D
        add    esp, 8
        or     eax, eax
        mov    edi, 1
        jne    SHORT loc_2_0A4A
loc_2_0A48:
        sub    edi, edi
loc_2_0A4A:
        or     edi, edi
        je     SHORT loc_2_0A95
        lea    eax, [ebp - 84h]
        push   eax
        push   ebx
        call   sub_2_0C99
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_0A95
        push   dword ptr [esi + 3ch]
        push   esi
        call   sub_2_0A9E
        add    esp, 8
        push   esi
        call   sub_2_07DB
        add    esp, 4
        mov    ecx, dword ptr [esi + 0d7h]
        push   dword ptr [ecx + 13h]
        push   dword ptr [esi + 0cfh]
        lea    eax, [ecx + 2bh]
        push   eax
        push   ecx
        call   sub_2_1FE0
        add    esp, 10h
loc_2_0A95:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0A9E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 0d7h]
        movzx  eax, word ptr [eax + 51h]
        sub    eax, edi
        cmp    eax, 1
        sbb    ebx, ebx
        neg    ebx
        or     ebx, ebx
        jne    NEAR PTR loc_2_0BA1
        cmp    edi, 4b0h
        je     SHORT loc_2_0ADF
        cmp    edi, 2580h
        je     SHORT loc_2_0ADF
        sub    ebx, ebx
        jmp    NEAR PTR loc_2_0BA1
loc_2_0ADF:
        test   byte ptr [esi + 0a3h], 2
        jne    SHORT loc_2_0AEF
        sub    ebx, ebx
        jmp    NEAR PTR loc_2_0BA1
loc_2_0AEF:
        push   edi
        call   sub_2_1F3C
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 1
        sbb    ebx, ebx
        inc    ebx
        je     NEAR PTR loc_2_0BA1
        mov    ecx, dword ptr [esi + 0d7h]
        cmp    word ptr [ecx + 51h], 4b0h
        jne    SHORT loc_2_0B4E
        push   dword ptr [eax + 4]
        add    ecx, 2bh
        push   ecx
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0B8D
        push   0ah
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 43h
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   edi
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_1F06
        jmp    SHORT loc_2_0B88
loc_2_0B4E:
        push   edi
        add    ecx, 2bh
        push   ecx
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0B8D
        push   0ah
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 43h
        push   eax
        call   sub_2_2554
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_2137
loc_2_0B88:
        add    esp, 8
        mov    ebx, eax
loc_2_0B8D:
        or     ebx, ebx
        je     SHORT loc_2_0BA1
        movzx  eax, di
        mov    dword ptr [esi + 3ch], eax
        mov    eax, dword ptr [esi + 0d7h]
        mov    word ptr [eax + 51h], di
loc_2_0BA1:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0BAA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [eax + 4]
        lea    edi, [ecx + 28h]
        movsd
        movsd
        movsd
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [ecx], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_0BCD:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        mov    ax, word ptr [ecx]
        cmp    ax, 80h
        ja     SHORT loc_2_0BE9
        movzx  eax, ax
        mov    dl, byte ptr [ebp + 8]
        mov    byte ptr [eax + ecx + 2], dl
        inc    word ptr [ecx]
loc_2_0BE9:
        pop    ebp
        ret
sub_2_0BEB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        add    eax, 18h
        mov    edi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [edi]
        sub    ebx, ebx
        push   eax
        call   sub_2_2570
        add    esp, 8
        mov    esi, dword ptr [ebp + 0ch]
loc_2_0C17:
        lea    eax, [ebp - 1]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_20FD
        add    esp, 8
        or     al, al
        je     SHORT loc_2_0C6D
        push   esi
        push   dword ptr [ebp - 1]
        call   dword ptr [edi + 8]
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, 3
        je     SHORT loc_2_0C5B
        cmp    ebx, 1
        je     SHORT loc_2_0C4C
        push   esi
        push   dword ptr [ebp - 1]
        call   sub_2_0BCD
        add    esp, 8
loc_2_0C4C:
        push   dword ptr [edi]
        push   dword ptr [ebp - 8]
        call   sub_2_2570
        add    esp, 8
        jmp    SHORT loc_2_0C7B
loc_2_0C5B:
        cmp    word ptr [esi], 0
        je     SHORT loc_2_0C6D
        mov    word ptr [esi], 0
        mov    ebx, 1
        jmp    SHORT loc_2_0C7B
loc_2_0C6D:
        push   dword ptr [ebp - 8]
        call   sub_2_2582
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
loc_2_0C7B:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_0C8B
        cmp    ebx, 2
        je     SHORT loc_2_0C8B
        cmp    ebx, 1
        jne    SHORT loc_2_0C17
loc_2_0C8B:
        cmp    word ptr [esi], 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_0C99:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_2122
        add    esp, 8
        lea    esi, [edi + 18h]
        push   96h
        push   esi
        call   sub_2_2554
        add    esp, 8
        mov    word ptr [edi + 26h], 4b0h
        push   1
        push   edi
        call   sub_2_2122
        add    esp, 8
        push   96h
        push   esi
        call   sub_2_2554
        add    esp, 8
        push   edi
        call   sub_2_2096
        add    esp, 4
        push   3
        push   edi
        call   sub_2_2122
        add    esp, 8
        db  0FFh, 035h, 044h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_2_0BEB
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_0D06:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_0BAA
        add    esp, 8
        push   edi
        push   esi
        call   sub_2_1EA0
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_0D7F
        push   dword ptr [ebp + 10h]
        call   sub_2_09D5
        add    esp, 4
        push   4b0h
        push   esi
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0D5D
        push   8
        push   esi
        call   sub_2_1F5D
        add    esp, 8
        or     eax, eax
        mov    ebx, 1
        jne    SHORT loc_2_0D5F
loc_2_0D5D:
        sub    ebx, ebx
loc_2_0D5F:
        or     ebx, ebx
        je     SHORT loc_2_0D7F
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_2_0C99
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_2_0D7F
        push   edi
        push   esi
        call   sub_2_1E74
        add    esp, 8
loc_2_0D7F:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_0D86:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    al, byte ptr [eax + 2]
        sub    al, 42h
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        ret
sub_2_0D99:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   14h
        lea    ebx, [edi + 18h]
        push   ebx
        call   sub_2_2554
        mov    esi, dword ptr [ebp + 10h]
        add    esp, 8
        push   dword ptr [esi + 4]
        push   edi
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_2_0DE9
        push   14h
        push   ebx
        call   sub_2_2554
        add    esp, 8
        push   edi
        call   sub_2_2096
        add    esp, 4
        push   esi
        push   dword ptr [ebp + 8]
        push   edi
        call   sub_2_0BEB
        add    esp, 0ch
loc_2_0DE9:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_0DEE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [eax]
        movzx  eax, cx
        or     eax, eax
        je     SHORT loc_2_0E1A
        test   byte ptr [ebp + 8], 0c0h
        je     SHORT loc_2_0E06
        jmp    SHORT loc_2_0E28
loc_2_0E06:
        cmp    cx, 3
        jne    SHORT loc_2_0E13
        mov    eax, 2
        jmp    SHORT loc_2_0E2D
loc_2_0E13:
        mov    eax, 4
        jmp    SHORT loc_2_0E2D
loc_2_0E1A:
        mov    al, byte ptr [ebp + 8]
        and    al, 0c0h
        cmp    al, 40h
        mov    eax, 4
        je     SHORT loc_2_0E2D
loc_2_0E28:
        mov    eax, 3
loc_2_0E2D:
        pop    ebp
        ret
sub_2_0E2F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi], 4
        jne    SHORT loc_2_0E8E
        mov    al, byte ptr [esi + 2]
        mov    edx, dword ptr [ebp + 0ch]
        and    ax, 3fh
        mov    word ptr [edx + 50h], ax
        mov    al, byte ptr [esi + 3]
        and    ax, 7
        mov    word ptr [edx + 52h], ax
        mov    cl, byte ptr [esi + 3]
        shr    cl, 3
        mov    eax, dword ptr [edx + 7bh]
        movzx  ecx, cl
        xor    ecx, eax
        and    ecx, 1
        xor    ecx, eax
        mov    dword ptr [edx + 7bh], ecx
        mov    al, byte ptr [esi + 3]
        shr    al, 4
        mov    dword ptr [edx + 4ch], 3
        movzx  eax, al
        add    eax, eax
        xor    eax, ecx
        and    eax, 2
        xor    eax, ecx
        mov    dword ptr [edx + 7bh], eax
        mov    eax, 1
        jmp    SHORT loc_2_0E90
loc_2_0E8E:
        sub    eax, eax
loc_2_0E90:
        pop    esi
        pop    ebp
        ret
loc_2_0E93:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [eax]
        movzx  eax, cx
        or     eax, eax
        je     SHORT loc_2_0EBF
        test   byte ptr [ebp + 8], 0c0h
        je     SHORT loc_2_0EAB
        jmp    SHORT loc_2_0ECD
loc_2_0EAB:
        cmp    cx, 6
        jne    SHORT loc_2_0EB8
        mov    eax, 2
        jmp    SHORT loc_2_0ED2
loc_2_0EB8:
        mov    eax, 4
        jmp    SHORT loc_2_0ED2
loc_2_0EBF:
        mov    al, byte ptr [ebp + 8]
        and    al, 0c0h
        cmp    al, 40h
        mov    eax, 4
        je     SHORT loc_2_0ED2
loc_2_0ECD:
        mov    eax, 3
loc_2_0ED2:
        pop    ebp
        ret
sub_2_0ED4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax], 7
        jne    SHORT loc_2_0F1D
        mov    cl, byte ptr [eax + 2]
        and    cl, 3fh
        cmp    cl, 1
        jne    SHORT loc_2_0F1D
        mov    dl, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp + 0ch]
        and    dx, 3
        mov    word ptr [ecx + 65h], dx
        mov    dl, byte ptr [eax + 4]
        and    dx, 7
        mov    word ptr [ecx + 67h], dx
        mov    al, byte ptr [eax + 3]
        and    al, 0ch
        shr    al, 2
        push   ecx
        push   eax
        call   sub_2_3FD6
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_2_0F1F
loc_2_0F1D:
        sub    eax, eax
loc_2_0F1F:
        pop    ebp
        ret
loc_2_0F21:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [eax]
        or     eax, eax
        je     SHORT loc_2_0F42
        cmp    eax, 1
        je     SHORT loc_2_0F62
        test   byte ptr [ebp + 8], 0c0h
        je     SHORT loc_2_0F3B
        jmp    SHORT loc_2_0F72
loc_2_0F3B:
        mov    eax, 4
        jmp    SHORT loc_2_0F77
loc_2_0F42:
        mov    al, byte ptr [ebp + 8]
        cmp    al, 4dh
        jne    SHORT loc_2_0F50
        mov    eax, 4
        jmp    SHORT loc_2_0F77
loc_2_0F50:
        cmp    al, 42h
        jne    SHORT loc_2_0F5B
        mov    eax, 2
        jmp    SHORT loc_2_0F77
loc_2_0F5B:
        mov    eax, 3
        jmp    SHORT loc_2_0F77
loc_2_0F62:
        mov    al, byte ptr [ebp + 8]
        cmp    al, 31h
        jb     SHORT loc_2_0F72
        cmp    al, 34h
        mov    eax, 4
        jbe    SHORT loc_2_0F77
loc_2_0F72:
        mov    eax, 1
loc_2_0F77:
        pop    ebp
        ret
sub_2_0F79:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
loc_2_0F7F:
        mov    cl, byte ptr [eax]
        inc    eax
        test   cl, 10h
        jne    SHORT loc_2_0F7F
        pop    ebp
        ret
sub_2_0F89:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    ecx, ecx
        sub    ax, ax
        mov    edx, dword ptr [ebp + 8]
loc_2_0F96:
        movzx  si, byte ptr [edx]
        mov    di, si
        and    di, 0fh
        shl    di, cl
        or     ax, di
        add    ecx, 4
        inc    edx
        test   si, 10h
        jne    SHORT loc_2_0F96
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_0FB6:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, 0
        test   byte ptr [edi], 20h
        je     NEAR PTR loc_2_10B4
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   87h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0FDC:
        add    esp, 8
        mov    dword ptr [esi + 83h], eax
        mov    ebx, eax
        push   ebx
        call   sub_2_409F
        add    esp, 4
        mov    al, byte ptr [edi]
        and    al, 0fh
        mov    byte ptr [ebx + 5ah], al
        inc    edi
        push   edi
        call   sub_2_0F89
        add    esp, 4
        mov    word ptr [ebx + 52h], ax
        push   edi
        call   sub_2_0F79
        add    esp, 4
        mov    edi, eax
        movzx  eax, word ptr [ebx + 52h]
        push   eax
        push   ebx
        call   sub_2_4026
        add    esp, 8
        mov    al, byte ptr [edi]
        and    al, 2
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    word ptr [ebx + 69h], ax
        mov    al, byte ptr [edi]
        and    al, 2
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    word ptr [esi + 69h], ax
        mov    al, byte ptr [edi]
        and    al, 4
        sub    al, 4
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebx + 73h], eax
        mov    al, byte ptr [edi]
        and    al, 4
        sub    al, 4
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi + 73h], eax
        mov    al, byte ptr [edi]
        and    al, 8
        sub    al, 8
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebx + 6fh], eax
        mov    al, byte ptr [edi]
        and    al, 8
        sub    al, 8
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi + 6fh], eax
        mov    al, byte ptr [edi]
        and    al, 10h
        sub    al, 10h
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebx + 6bh], eax
        mov    al, byte ptr [edi]
        and    al, 10h
        sub    al, 10h
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [esi + 6bh], eax
        mov    al, byte ptr [edi + 1]
        inc    edi
        and    ax, 7
        mov    word ptr [ebx + 67h], ax
        mov    al, byte ptr [edi]
        and    ax, 7
        mov    word ptr [esi + 67h], ax
        mov    eax, ebx
loc_2_10B4:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_10B9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, 2
        movzx  esi, word ptr [eax]
        lea    edx, [esi - 1]
        cmp    edx, 2
        jle    SHORT loc_2_10E8
        mov    ebx, 20h
loc_2_10D6:
        movzx  edi, cx
        test   byte ptr [eax + edi + 2], bl
        je     SHORT loc_2_10F3
        inc    cx
        movzx  edi, cx
        cmp    edi, edx
        jl     SHORT loc_2_10D6
loc_2_10E8:
        test   byte ptr [esi + eax + 1], 30h
        je     SHORT loc_2_10F7
        sub    eax, eax
        jmp    SHORT loc_2_10FC
loc_2_10F3:
        sub    eax, eax
        jmp    SHORT loc_2_10FC
loc_2_10F7:
        mov    eax, 1
loc_2_10FC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_1101:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_10B9
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_111B
        sub    eax, eax
        jmp    SHORT loc_2_1190
loc_2_111B:
        lea    ebx, [esi + 4]
        mov    edi, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_0F89
        add    esp, 4
        mov    word ptr [edi + 52h], ax
        mov    dword ptr [edi + 4ch], 4
        movzx  eax, ax
        push   eax
        push   edi
        call   sub_2_4026
        add    esp, 8
        push   ebx
        call   sub_2_0F79
        add    esp, 4
        mov    ebx, eax
        push   ebx
        call   sub_2_0F89
        add    esp, 4
        mov    word ptr [edi + 50h], ax
        push   ebx
        call   sub_2_0F79
        add    esp, 4
        mov    ebx, eax
        movzx  eax, word ptr [esi]
        add    eax, esi
        inc    eax
        cmp    eax, ebx
        jbe    SHORT loc_2_118B
        push   edi
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        jne    SHORT loc_2_118B
        push   ebx
        push   edi
        call   sub_2_0FB6
        add    esp, 8
        or     dword ptr [edi + 7bh], 2
loc_2_118B:
        mov    eax, 1
loc_2_1190:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_1195:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cmp    dword ptr [ebp + 10h], 0
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_2_11B8
        mov    ax, word ptr [esi]
        cmp    ax, 4
        jb     SHORT loc_2_11B4
        jne    SHORT loc_2_11B8
        test   byte ptr [esi + 5], 20h
        je     SHORT loc_2_11B8
loc_2_11B4:
        sub    eax, eax
        jmp    SHORT loc_2_1214
loc_2_11B8:
        lea    eax, [esi + 2]
        cmp    byte ptr [eax], 4dh
        je     SHORT loc_2_11C4
        sub    eax, eax
        jmp    SHORT loc_2_1214
loc_2_11C4:
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 4ch], 1
        mov    cl, byte ptr [esi + 3]
        or     cl, cl
        jne    SHORT loc_2_11D9
        sub    eax, eax
        jmp    SHORT loc_2_1214
loc_2_11D9:
        and    cl, 0fh
        mov    byte ptr [edi + 5ah], cl
        mov    dword ptr [edi + 4ch], 2
        movzx  ecx, word ptr [esi]
        push   ecx
        push   eax
        lea    eax, [edi + 7fh]
        push   eax
        call   sub_2_2804
        add    esp, 0ch
        or     eax, eax
        push   edi
        je     SHORT loc_2_120B
        call   sub_2_28BC
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_2_1214
loc_2_120B:
        push   esi
        call   sub_2_1101
        add    esp, 8
loc_2_1214:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1218:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_09D5
        add    esp, 4
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_2_0D99
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_1253
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_0E2F
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_2_1255
loc_2_1253:
        sub    eax, eax
loc_2_1255:
        pop    esi
        pop    ebp
        ret
sub_2_1258:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_09D5
        add    esp, 4
        db  0FFh, 035h, 040h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_2_0D99
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_1293
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_0ED4
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_2_1295
loc_2_1293:
        sub    eax, eax
loc_2_1295:
        pop    esi
        pop    ebp
        ret
sub_2_1298:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_09D5
        add    esp, 4
        db  0FFh, 035h, 044h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_2_0D99
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_12D5
        push   1
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_1195
        add    esp, 0ch
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_2_12D7
loc_2_12D5:
        sub    eax, eax
loc_2_12D7:
        pop    esi
        pop    ebp
        ret
sub_2_12DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_2_409F
        add    esp, 4
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_0D86
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_1300
        sub    eax, eax
        jmp    SHORT loc_2_1375
loc_2_1300:
        lea    eax, [edi + 54h]
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ebx + 30h]
        push   dword ptr [ebx + 28h]
        push   2
        call   sub_2_3DEC
        add    esp, 10h
        mov    dword ptr [edi + 56h], eax
        push   0
        push   edi
        push   esi
        call   sub_2_1195
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_1341
        push   edi
        call   sub_2_3F4B
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_1370
        push   ebx
        push   edi
        push   esi
        call   sub_2_1298
        jmp    SHORT loc_2_136D
loc_2_1341:
        push   ebx
        push   edi
        push   esi
        call   sub_2_1298
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_2_1370
        push   ebx
        push   edi
        push   esi
        call   sub_2_1218
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_1370
        test   byte ptr [edi + 7bh], 1
        je     SHORT loc_2_1370
        push   ebx
        push   edi
        push   esi
        call   sub_2_1258
loc_2_136D:
        add    esp, 0ch
loc_2_1370:
        mov    eax, 1
loc_2_1375:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_137A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [edx + 2bh]
        mov    edi, eax
        mov    ecx, 0dh
        rep movsd
        mov    ax, word ptr [eax]
        mov    word ptr [edx + 0ch], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_13A1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [edi + 0d7h]
        call   sub_2_2050
        add    esp, 4
        lea    esi, [edi + 0afh]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_13EA
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 0cfh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 0d3h], eax
        mov    eax, 1
        jmp    SHORT loc_2_13EC
loc_2_13EA:
        sub    eax, eax
loc_2_13EC:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_13F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   2
        mov    eax, dword ptr [esi + 0d7h]
        mov    eax, dword ptr [eax + 5bh]
        mov    dword ptr [esi + 5ch], eax
        mov    eax, dword ptr [esi + 0d7h]
        mov    eax, dword ptr [eax + 53h]
        mov    dword ptr [esi + 38h], eax
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 58h], eax
        mov    dword ptr [esi + 28h], 2
        mov    dword ptr [esi + 68h], 64h
        mov    dword ptr [esi + 6ch], 8
        mov    eax, dword ptr [esi + 0d7h]
        mov    dword ptr [esi + 3ch], 4b0h
        movzx  eax, word ptr [eax + 0ch]
        mov    dword ptr [esi + 44h], eax
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_1450:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   5fh
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_2_14C6
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 0d7h]
        call   sub_2_137A
        add    esp, 8
        push   esi
        call   sub_2_13A1
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_14C6
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [esi + 0cfh]
        push   eax
        add    eax, 2bh
        push   eax
        call   sub_2_1F89
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_2_14B7
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_2_13F0
        add    esp, 8
        mov    edi, 1
        jmp    SHORT loc_2_14C6
loc_2_14B7:
        add    esi, 0afh
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_14C6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_14CC:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0d7h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 13h], eax
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 20h
        mov    edx, dword ptr [ecx + 0d7h]
        mov    dword ptr [edx + 27h], eax
        mov    eax, dword ptr [ecx + 0d7h]
        mov    dword ptr [eax + 1bh], 0
        mov    eax, dword ptr [ecx + 0d7h]
        mov    dword ptr [eax + 17h], ecx
        mov    edx, dword ptr [ecx + 0d7h]
        movzx  eax, word ptr [ecx + 7ah]
        mov    dword ptr [edx + 1fh], eax
        mov    eax, dword ptr [ecx + 0d7h]
        mov    edx, dword ptr [ecx + 38h]
        mov    dword ptr [eax + 23h], edx
        mov    eax, 1
        pop    ebp
        ret
sub_2_1528:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 018h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 020h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
sub_2_1551:
        push   ebp
        mov    ebp, esp
        sub    esp, 0b8h
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        lea    eax, [ebp - 0b8h]
        push   eax
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_2_0D06
        add    esp, 0ch
        cmp    eax, esi
        je     NEAR PTR loc_2_1638
        mov    ebx, dword ptr [ebp + 8]
        lea    eax, [ebp - 0b8h]
        lea    edi, [ebx + 28h]
        push   edi
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_2_12DA
        add    esp, 0ch
        cmp    eax, esi
        je     NEAR PTR loc_2_1625
        push   edi
        lea    eax, [ebp - 34h]
        push   eax
        push   ebx
        call   sub_2_1450
        add    esp, 0ch
        cmp    eax, esi
        je     SHORT loc_2_1625
        push   ebx
        call   sub_2_1528
        add    esp, 4
        push   ebx
        call   sub_2_4333
        add    esp, 4
        push   ebx
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [ebx + 2ch]
        push   ebx
        call   sub_2_9371
        add    esp, 8
        push   edi
        call   sub_2_3F4B
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_15F3
        call   sub_2_80A2
        cmp    eax, esi
        je     SHORT loc_2_15F3
        mov    esi, 1
loc_2_15F3:
        push   esi
        push   ebx
        call   sub_2_14CC
        add    esp, 8
        push   ebx
        call   sub_2_07DB
        add    esp, 4
        mov    ecx, dword ptr [ebx + 0d7h]
        push   esi
        push   dword ptr [ebx + 0cfh]
        lea    eax, [ecx + 2bh]
        push   eax
        push   ecx
        call   sub_2_1FE0
        add    esp, 10h
        mov    esi, 1
loc_2_1625:
        or     esi, esi
        jne    SHORT loc_2_1638
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_2_1E74
        add    esp, 8
loc_2_1638:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_2_1644:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 8]
        call   sub_2_0488
        add    esp, 4
        push   esi
        call   sub_2_07B4
        add    esp, 4
        push   dword ptr [esi + 38h]
        call   sub_2_585C
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_1E74
        add    esp, 8
        lea    eax, [esi + 0afh]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_2_16BD:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 0ch], 0
        mov    eax, 0
        jne    SHORT loc_2_16D9
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_2_16DE
        add    esp, 8
loc_2_16D9:
        pop    ebp
        ret
loc_2_16DB:
        sub    eax, eax
        ret
sub_2_16DE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 0d7h]
        movzx  eax, word ptr [eax + 51h]
        cmp    eax, edi
        jne    SHORT loc_2_16FE
        mov    eax, 1
        jmp    SHORT loc_2_177C
loc_2_16FE:
        push   edi
        call   sub_2_1F3C
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_177A
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_177A
        push   0ah
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 43h
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   edi
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_177A
        movzx  eax, di
        mov    dword ptr [esi + 3ch], eax
        mov    eax, dword ptr [esi + 0d7h]
        push   0ah
        mov    word ptr [eax + 51h], di
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 43h
        push   eax
        call   sub_2_2554
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_2_177C
loc_2_177A:
        sub    eax, eax
loc_2_177C:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_1780:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [eax + 0d7h]
        add    esi, 2bh
        push   0
        push   esi
        call   sub_2_1F5D
        mov    eax, dword ptr [ebp + 8]
        add    esp, 8
        mov    eax, dword ptr [eax + 0d7h]
        push   dword ptr [eax + 8]
        call   sub_2_0458
        add    esp, 4
        push   0
        push   esi
        call   sub_2_2122
        add    esp, 8
        lea    edi, [esi + 18h]
        push   1f4h
        push   edi
        call   sub_2_2554
        add    esp, 8
        push   3
        push   esi
        call   sub_2_2122
        add    esp, 8
        push   1f4h
        push   edi
        call   sub_2_2554
        add    esp, 8
        db  0BBh, 050h, 000h, 000h, 000h
loc_2_17F5:
        mov    edi, dword ptr [ebx]
        add    ebx, 4
        push   edi
        push   esi
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_1818
        mov    word ptr [esi + 26h], di
        push   esi
        call   sub_2_18D8
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_2_1818:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_182C
        cmp    edi, 2580h
        jne    SHORT loc_2_17F5
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1867
loc_2_182C:
        push   dword ptr [ebp + 8]
        call   sub_2_1C90
        add    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_2_07DB
        mov    eax, dword ptr [ebp + 8]
        add    esp, 4
        mov    ecx, dword ptr [eax + 0d7h]
        push   0
        push   dword ptr [eax + 0cfh]
        lea    eax, [ecx + 2bh]
        push   eax
        push   ecx
        call   sub_2_1FE0
        add    esp, 10h
        mov    eax, 1
        jmp    SHORT loc_2_1869
loc_2_1867:
        sub    eax, eax
loc_2_1869:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1870:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [eax + 4]
        lea    edi, [ecx + 28h]
        movsd
        movsd
        movsd
        mov    eax, dword ptr [eax + 8]
        mov    word ptr [ecx], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1893:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0FFh, 035h, 048h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2137
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_18BE
        push   2
        add    esi, 18h
        push   esi
        call   sub_2_2554
        add    esp, 8
loc_2_18BE:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_18C4:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        sub    al, 4fh
        cmp    al, 1
        pop    ebp
        sbb    eax, eax
        and    eax, 1
        add    eax, 3
        ret
sub_2_18D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1893
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_1971
        push   edi
        call   sub_2_2096
        add    esp, 4
        push   64h
        lea    eax, [edi + 18h]
        push   eax
        call   sub_2_2570
        add    esp, 8
        db  0FFh, 035h, 04Ch, 000h, 000h, 000h
        push   edi
        call   sub_2_2137
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_2_1973
        push   64h
        lea    eax, [edi + 18h]
        push   eax
        call   sub_2_2570
        add    esp, 8
        mov    ebx, dword ptr [ebp - 8]
loc_2_192E:
        lea    eax, [ebp - 1]
        push   eax
        push   edi
        call   sub_2_20FD
        add    esp, 8
        or     al, al
        je     SHORT loc_2_194E
        push   dword ptr [ebp - 1]
        call   sub_2_18C4
        add    esp, 4
        mov    ebx, eax
        jmp    SHORT loc_2_195C
loc_2_194E:
        lea    eax, [edi + 18h]
        push   eax
        call   sub_2_2582
        add    esp, 4
        mov    esi, eax
loc_2_195C:
        or     esi, esi
        jne    SHORT loc_2_1965
        cmp    ebx, 4
        jne    SHORT loc_2_192E
loc_2_1965:
        sub    ebx, 4
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_2_1973
loc_2_1971:
        sub    eax, eax
loc_2_1973:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_197A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        push   dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1870
        add    esp, 8
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_2_1EA0
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_1A3C
        push   0
        push   edi
        call   sub_2_1F5D
        add    esp, 8
        push   0
        push   edi
        call   sub_2_2122
        add    esp, 8
        lea    esi, [edi + 18h]
        push   1f4h
        push   esi
        call   sub_2_2554
        add    esp, 8
        push   3
        push   edi
        call   sub_2_2122
        add    esp, 8
        push   1f4h
        push   esi
        call   sub_2_2554
        add    esp, 8
        db  0BEh, 050h, 000h, 000h, 000h
loc_2_19F2:
        mov    ebx, dword ptr [esi]
        add    esi, 4
        push   ebx
        push   edi
        call   sub_2_1F06
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_1A15
        mov    word ptr [edi + 26h], bx
        push   edi
        call   sub_2_18D8
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
loc_2_1A15:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_1A29
        cmp    ebx, 2580h
        jne    SHORT loc_2_19F2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1A30
loc_2_1A29:
        mov    eax, 1
        jmp    SHORT loc_2_1A3E
loc_2_1A30:
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_2_1E74
        add    esp, 8
loc_2_1A3C:
        sub    eax, eax
loc_2_1A3E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_1A45:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [edx + 2bh]
        mov    edi, eax
        mov    ecx, 0dh
        rep movsd
        mov    ax, word ptr [eax]
        mov    word ptr [edx + 0ch], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1A6C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [edi + 0d7h]
        call   sub_2_2050
        add    esp, 4
        lea    esi, [edi + 0afh]
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_1AB3
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 0cfh], eax
        db  0C7h, 087h, 0D3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_2_1AB5
loc_2_1AB3:
        sub    eax, eax
loc_2_1AB5:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1AB9:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   2
        mov    eax, dword ptr [esi + 0d7h]
        mov    byte ptr [esi + 82h], 3
        mov    eax, dword ptr [eax + 5bh]
        mov    dword ptr [esi + 5ch], eax
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 58h], eax
        mov    eax, dword ptr [esi + 0d7h]
        mov    eax, dword ptr [eax + 53h]
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 28h], 2
        mov    dword ptr [esi + 2ch], 4
        mov    dword ptr [esi + 74h], 7
        mov    dword ptr [esi + 6ch], 5
        mov    dword ptr [esi + 3ch], 960h
        mov    eax, dword ptr [esi + 0d7h]
        mov    dword ptr [esi + 68h], 96h
        movzx  eax, word ptr [eax + 0ch]
        mov    dword ptr [esi + 44h], eax
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_1B2E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   5fh
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_2_1BA4
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 0d7h]
        call   sub_2_1A45
        add    esp, 8
        push   esi
        call   sub_2_1A6C
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_1BA4
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [esi + 0cfh]
        push   eax
        add    eax, 2bh
        push   eax
        call   sub_2_1F89
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_2_1B95
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_2_1AB9
        add    esp, 8
        mov    edi, 1
        jmp    SHORT loc_2_1BA4
loc_2_1B95:
        add    esi, 0afh
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_1BA4:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1BAA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        call   sub_2_1F1E
        add    esp, 4
        mov    ecx, eax
        cmp    ecx, 1
        sbb    eax, eax
        inc    eax
        je     SHORT loc_2_1C12
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ecx + 4]
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_2137
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_1C12
        push   0ah
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 43h
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   edi
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_1F5D
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_1C12
        movzx  ecx, di
        mov    dword ptr [esi + 6ch], ecx
loc_2_1C12:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1C16:
        push   ebp
        mov    ebp, esp
        db  0B8h, 010h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
loc_2_1C21:
        mov    ecx, dword ptr [eax + 8]
        add    eax, 8
        cmp    ecx, edx
        je     SHORT loc_2_1C35
        cmp    ecx, 96h
        jne    SHORT loc_2_1C21
        sub    eax, eax
loc_2_1C35:
        pop    ebp
        ret
sub_2_1C37:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_1C16
        add    esp, 4
        cmp    eax, 1
        sbb    edi, edi
        inc    edi
        je     SHORT loc_2_1C89
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 2bh
        push   eax
        call   sub_2_2137
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_1C89
        movzx  eax, bx
        mov    dword ptr [esi + 68h], eax
        push   0ah
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 43h
        push   eax
        call   sub_2_2554
        add    esp, 8
loc_2_1C89:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_1C90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   960h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_16DE
        add    esp, 8
        push   5
        push   esi
        call   sub_2_1BAA
        add    esp, 8
        push   96h
        push   esi
        call   sub_2_1C37
        add    esp, 8
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_1CC6:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 018h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 020h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
sub_2_1CEF:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        lea    ebx, [esi + 28h]
        push   ebx
        call   sub_2_409F
        add    esp, 4
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_2_197A
        add    esp, 8
        cmp    eax, edi
        je     NEAR PTR loc_2_1DA4
        push   ebx
        lea    eax, [ebp - 34h]
        push   eax
        push   esi
        call   sub_2_1B2E
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_2_1D95
        push   esi
        call   sub_2_1C90
        add    esp, 4
        push   esi
        call   sub_2_1CC6
        add    esp, 4
        push   esi
        call   sub_2_4333
        add    esp, 4
        push   esi
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [esi + 2ch]
        push   esi
        call   sub_2_9371
        add    esp, 8
        mov    eax, dword ptr [esi + 0d7h]
        push   esi
        mov    dword ptr [eax + 13h], edi
        call   sub_2_07DB
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        push   edi
        push   dword ptr [esi + 0cfh]
        lea    ecx, [eax + 2bh]
        push   ecx
        push   eax
        call   sub_2_1FE0
        add    esp, 10h
        mov    edi, 1
        jmp    SHORT loc_2_1DA4
loc_2_1D95:
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_2_1E74
        add    esp, 8
loc_2_1DA4:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_2_1DB0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        cmp    dword ptr [ecx + 28h], 2
        jne    SHORT loc_2_1DCB
        mov    ecx, dword ptr [ecx + 38h]
        sub    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
loc_2_1DCB:
        pop    ebp
        ret
sub_2_1DCD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        sub    esi, esi
        push   dword ptr [ebx + 4]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        cmp    eax, esi
        jne    SHORT loc_2_1E0B
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_1551
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_1E0B
        push   ebx
        push   edi
        call   sub_2_1CEF
        add    esp, 8
        mov    esi, eax
loc_2_1E0B:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_1E12:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        test   byte ptr [ecx + 8], 1
        je     SHORT loc_2_1E52
        mov    edx, dword ptr [ecx + 4]
        cmp    edx, eax
        je     SHORT loc_2_1E40
        mov    ecx, dword ptr [ecx + 10h]
loc_2_1E2E:
        cmp    dword ptr [edx + 4], ecx
        je     SHORT loc_2_1E3B
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_2_1E2E
        jmp    SHORT loc_2_1E40
loc_2_1E3B:
        mov    esi, 1
loc_2_1E40:
        or     esi, esi
        je     SHORT loc_2_1E70
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_2_1DCD
        add    esp, 8
        jmp    SHORT loc_2_1E70
loc_2_1E52:
        mov    edi, dword ptr [ecx + 4]
        mov    esi, dword ptr [ebp + 8]
loc_2_1E58:
        or     edi, edi
        je     SHORT loc_2_1E70
        push   edi
        push   esi
        call   sub_2_1DCD
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_1E70
        mov    edi, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_2_1E58
loc_2_1E70:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1E74:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2030
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 22h]
        or     eax, eax
        je     SHORT loc_2_1E9A
        push   dword ptr [esi + 28h]
        push   eax
        call   sub_2_21C6
        add    esp, 8
loc_2_1E9A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        call   sub_2_2066
        cmp    eax, 30bh
        jb     SHORT loc_2_1F00
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 2ch]
        call   sub_2_21A7
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_1ED7
        push   dword ptr [esi + 28h]
        call   sub_2_21D8
        add    esp, 4
        mov    dword ptr [esi + 22h], eax
        cmp    eax, edi
        je     SHORT loc_2_1F00
loc_2_1ED7:
        push   esi
        call   sub_2_2071
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_2_1F00
        mov    eax, dword ptr [esi + 22h]
        or     eax, eax
        je     SHORT loc_2_1F00
        push   dword ptr [esi + 28h]
        push   eax
        call   sub_2_21C6
        add    esp, 8
        mov    dword ptr [esi + 22h], 0
loc_2_1F00:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_1F06:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_20AA
        add    esp, 8
        mov    eax, 1
        pop    ebp
        ret
sub_2_1F1E:
        push   ebp
        mov    ebp, esp
        db  0B8h, 00Fh, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
loc_2_1F29:
        mov    edx, dword ptr [eax + 9]
        add    eax, 9
        cmp    ecx, edx
        je     SHORT loc_2_1F3A
        cmp    edx, 8
        jne    SHORT loc_2_1F29
        sub    eax, eax
loc_2_1F3A:
        pop    ebp
        ret
sub_2_1F3C:
        push   ebp
        mov    ebp, esp
        db  0B8h, 048h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
loc_2_1F47:
        mov    edx, dword ptr [eax + 8]
        add    eax, 8
        cmp    ecx, edx
        je     SHORT loc_2_1F5B
        cmp    edx, 2580h
        jne    SHORT loc_2_1F47
        sub    eax, eax
loc_2_1F5B:
        pop    ebp
        ret
sub_2_1F5D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0ch]
        call   sub_2_1F1E
        add    esp, 4
        cmp    eax, 1
        sbb    esi, esi
        inc    esi
        je     SHORT loc_2_1F84
        movzx  eax, byte ptr [eax + 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_20C2
        add    esp, 8
loc_2_1F84:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
sub_2_1F89:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        push   dword ptr [eax + 30h]
        call   sub_2_0721
        add    esp, 0ch
        mov    dword ptr [esi + 8], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        pop    ebp
        ret
sub_2_1FAF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0bh
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2122
        add    esp, 8
        push   dword ptr [ebp + 14h]
        push   esi
        call   sub_2_20D9
        add    esp, 8
        push   dword ptr [ebp + 0ch]
        call   sub_2_0467
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_1FE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 4], 0ffffff9ch
        mov    byte ptr [ecx + 12h], 0
        mov    byte ptr [ecx + 0eh], 0
        mov    byte ptr [ecx + 0fh], 0
        mov    byte ptr [ecx + 10h], 0
        mov    byte ptr [ecx + 11h], 0
        push   dword ptr [ebp + 10h]
        push   dword ptr [ecx + 8]
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_1FAF
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_2027
        push   esi
        call   sub_2_2096
        add    esp, 4
loc_2_2027:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_2030:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_2298
        mov    eax, 1
        je     SHORT loc_2_204B
        mov    eax, 0
loc_2_204B:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_2_2050:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 000h, 06Ch, 003h, 000h, 000h
        leave
        ret
sub_2_205F:
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        ret
sub_2_2066:
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        jae    SHORT loc_2_2070
        xor    eax, eax
loc_2_2070:
        ret
sub_2_2071:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [edi]
        lea    edi, [edi]
        call   sub_2_24F2
        mov    eax, 1
        je     SHORT loc_2_2091
        mov    eax, 0
loc_2_2091:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_2_2096:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_247F
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_2_20AA:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        call   sub_2_221F
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_2_20C2:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0ch]
        call   sub_2_21F6
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_2_20D9:
        enter  0, 0
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, 1
        cmp    ebx, 0
        je     SHORT loc_2_20F4
        mov    eax, 9
loc_2_20F4:
        call   sub_2_2258
        pop    ebx
        pop    edi
        leave
        ret
sub_2_20FD:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_247B
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [edi], al
        mov    eax, 1
        je     SHORT loc_2_211D
        mov    eax, 0
loc_2_211D:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
sub_2_2122:
        enter  0, 0
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_2_2250
        pop    esi
        pop    edi
        leave
        ret
sub_2_2137:
        enter  0, 0
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        call   sub_2_2459
        mov    eax, 1
        je     SHORT loc_2_2154
        mov    eax, 0
loc_2_2154:
        pop    ebx
        pop    edi
        leave
        ret
sub_2_2158:
        pushad
        mov    ebp, esp
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        cmp    eax, 400h
        jb     SHORT loc_2_21A5
        mov    eax, ecx
        test   edx, edx
        je     SHORT loc_2_2180
        push   eax
        mov    eax, edx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 007h, 000h, 00Eh, 000h
        pop    eax
        jmp    SHORT loc_2_219F
loc_2_2180:
        mov    dword ptr [ebp + 1ch], edx
        push   eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   4
        pop    ecx
        xor    esi, esi
        xor    edi, edi
        db  0CDh, 020h, 006h, 000h, 00Eh, 000h
        mov    ebx, eax
        pop    eax
        jb     SHORT loc_2_21A5
        mov    dword ptr [ebp + 1ch], ebx
loc_2_219F:
        db  0CDh, 020h, 00Ah, 000h, 00Eh, 000h
loc_2_21A5:
        popad
        ret
sub_2_21A7:
        push   ebp
        mov    ebp, esp
        db  0CDh, 020h, 000h, 000h, 00Eh, 000h
        cmp    eax, 400h
        jb     SHORT loc_2_21C2
        mov    eax, dword ptr [ebp + 8]
        db  0CDh, 020h, 009h, 000h, 00Eh, 000h
        jae    SHORT loc_2_21C4
loc_2_21C2:
        xor    eax, eax
loc_2_21C4:
        leave
        ret
sub_2_21C6:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        call   sub_2_2158
        sbb    eax, eax
        leave
        ret
sub_2_21D8:
        push   ebp
        mov    ebp, esp
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_2158
        leave
        ret
        db  000h
sub_2_21E8:
        xor    dh, dh
        add    dx, word ptr [edi]
        in     al, dx
        ret
sub_2_21EF:
        xor    dh, dh
        add    dx, word ptr [edi]
        out    dx, al
        ret
sub_2_21F6:
        mov    dl, 3
        call   sub_2_21EF
        ret
sub_2_21FE:
        mov    dl, 3
        call   sub_2_21E8
        ret
sub_2_2206:
        or     al, al
        je     SHORT loc_2_220C
        mov    al, 0ffh
loc_2_220C:
        and    al, 80h
        push   eax
        call   sub_2_21FE
        and    al, 7fh
        pop    ecx
        or     al, cl
        call   sub_2_21F6
        ret
sub_2_221F:
        mov    cx, ax
        mov    dx, 1
        mov    ax, 0c200h
        div    cx
        push   eax
        mov    al, 0ffh
        call   sub_2_2206
        pop    eax
        push   eax
        mov    dl, 0
        call   sub_2_21EF
        pop    eax
        xchg   ah, al
        mov    dl, 1
        call   sub_2_21EF
        xor    al, al
        call   sub_2_2206
        ret
sub_2_2250:
        mov    dl, 4
        call   sub_2_21EF
        ret
sub_2_2258:
        mov    dl, 1
        call   sub_2_21EF
        ret
sub_2_2260:
        mov    dl, 2
        call   sub_2_21EF
        ret
sub_2_2268:
        mov    al, byte ptr [ebx + 6]
        call   sub_2_2258
        mov    al, byte ptr [ebx + 4]
        call   sub_2_21F6
        mov    al, byte ptr [ebx + 5]
        call   sub_2_2250
        mov    eax, dword ptr [ebx]
        call   sub_2_221F
        mov    cl, byte ptr [ebx + 7]
        mov    al, 1
        or     cl, cl
        jne    SHORT loc_2_2292
        mov    al, 0
loc_2_2292:
        call   sub_2_2260
        ret
sub_2_2298:
        push   edi
        push   ebx
        lea    ebx, [edi + 8]
        call   sub_2_2268
        xor    ebx, ebx
        pop    ebx
        pop    edi
        ret
sub_2_22A7:
        push   ebx
        xor    bl, bl
        test   al, 1eh
        je     SHORT loc_2_22D0
        inc    bl
        test   al, 2
        je     SHORT loc_2_22B8
        inc    word ptr [edi + 10h]
loc_2_22B8:
        test   al, 4
        je     SHORT loc_2_22C0
        inc    word ptr [edi + 14h]
loc_2_22C0:
        test   al, 8
        je     SHORT loc_2_22C8
        inc    word ptr [edi + 16h]
loc_2_22C8:
        test   al, 10h
        je     SHORT loc_2_22D0
        inc    word ptr [edi + 12h]
loc_2_22D0:
        or     bl, bl
        pop    ebx
        ret
sub_2_22D4:
        xor    ax, ax
        mov    word ptr [edi + 10h], ax
        mov    word ptr [edi + 12h], ax
        mov    word ptr [edi + 16h], ax
        mov    word ptr [edi + 14h], ax
        ret
        db  066h, 08Bh, 047h, 010h, 066h, 089h, 003h, 066h, 08Bh, 047h, 012h, 066h, 089h, 043h, 002h, 066h
        db  08Bh, 047h, 016h, 066h, 089h, 043h, 006h, 066h, 08Bh, 047h, 014h, 066h, 089h, 043h, 004h, 0C3h
sub_2_2308:
        mov    dl, 5
        call   sub_2_21E8
        push   eax
        call   sub_2_22A7
        pop    eax
        ret
sub_2_2317:
        call   sub_2_2308
        not    al
        and    al, 1
        ret
sub_2_2321:
        call   sub_2_2308
        not    al
        and    al, 20h
        ret
sub_2_232B:
        call   sub_2_2308
        not    al
        and    al, 40h
        ret
sub_2_2335:
        call   sub_2_2321
        jne    SHORT loc_2_2341
        call   sub_2_232B
loc_2_2341:
        ret
sub_2_2342:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     SHORT loc_2_2352
        call   sub_2_2375
        not    al
        and    al, 10h
loc_2_2352:
        ret
sub_2_2353:
        or     eax, eax
        je     SHORT loc_2_235C
        mov    eax, 0ffffffffh
loc_2_235C:
        mov    dword ptr [edi + 4], eax
        ret
sub_2_2360:
        call   sub_2_2335
        jne    SHORT loc_2_236C
        call   sub_2_2342
loc_2_236C:
        ret
sub_2_236D:
        mov    dl, 4
        call   sub_2_21E8
        ret
sub_2_2375:
        mov    dl, 6
        call   sub_2_21E8
        ret
sub_2_237D:
        mov    dl, 1
        call   sub_2_21E8
        ret
sub_2_2385:
        mov    dl, 2
        call   sub_2_21E8
        ret
sub_2_238D:
        mov    al, 0ffh
        call   sub_2_2206
        mov    dl, 1
        call   sub_2_21E8
        mov    ch, al
        mov    dl, 0
        call   sub_2_21E8
        mov    cl, al
        call   sub_2_23C6
        jne    SHORT loc_2_23B9
        mov    dx, 1
        mov    ax, 0c200h
        div    cx
        cwde
loc_2_23B9:
        push   eax
        xor    al, al
        call   sub_2_2206
        pop    eax
        xor    cx, cx
        ret
sub_2_23C6:
        cmp    cx, 2
        ja     SHORT loc_2_23D0
        mov    cx, 60h
loc_2_23D0:
        xor    ax, ax
        ret
sub_2_23D4:
        mov    dl, 2
        call   sub_2_21E8
        not    al
        test   al, 0c0h
        mov    al, 0
        jne    SHORT loc_2_23E5
        dec    al
loc_2_23E5:
        ret
        db  0B0h, 001h, 0E8h, 073h, 0FEh, 0FFh, 0FFh, 0E8h, 0E2h, 0FFh, 0FFh, 0FFh, 066h, 09Ch, 0B0h, 000h
        db  0E8h, 065h, 0FEh, 0FFh, 0FFh, 066h, 09Dh, 0C3h, 066h, 08Bh, 007h, 0C3h
sub_2_2402:
        mov    word ptr [edi], ax
        ret
sub_2_2406:
        mov    dl, 0
        call   sub_2_21E8
        ret
sub_2_240E:
        mov    word ptr [edi + 2], ax
        ret
sub_2_2413:
        mov    ax, word ptr [edi + 2]
        ret
sub_2_2418:
        mov    dl, 0
        call   sub_2_21EF
        ret
sub_2_2420:
        push   eax
        call   sub_2_2413
        push   edi
        lea    edi, [edi + 18h]
        push   eax
        push   edi
        call   sub_2_2570
        add    esp, 8
        pop    edi
loc_2_2435:
        call   sub_2_2360
        je     SHORT loc_2_244E
        push   edi
        lea    edi, [edi + 18h]
        push   edi
        call   sub_2_2582
        add    esp, 4
        pop    edi
        or     eax, eax
        je     SHORT loc_2_2435
loc_2_244E:
        pop    eax
        jne    SHORT loc_2_2458
        call   sub_2_2418
        xor    eax, eax
loc_2_2458:
        ret
sub_2_2459:
        mov    al, byte ptr [ebx]
        or     al, al
        je     SHORT loc_2_246B
        push   ebx
        call   sub_2_2420
        pop    ebx
        jne    SHORT loc_2_246B
        inc    ebx
        jmp    SHORT sub_2_2459
loc_2_246B:
        ret
loc_2_246C:
        call   sub_2_2317
        jne    SHORT loc_2_247A
        call   sub_2_2406
        xor    ah, ah
loc_2_247A:
        ret
sub_2_247B:
        call   dword ptr [edi + 1eh]
        ret
sub_2_247F:
        call   sub_2_247B
        je     SHORT sub_2_247F
        call   sub_2_2375
        ret
        db  057h, 08Dh, 07Fh, 018h, 050h, 057h, 0E8h, 0D9h, 000h, 000h, 000h, 083h, 0C4h, 008h, 05Fh, 0E8h
        db  0DBh, 0FFh, 0FFh, 0FFh, 074h, 012h, 057h, 08Dh, 07Fh, 018h, 057h, 0E8h, 0D6h, 000h, 000h, 000h
        db  083h, 0C4h, 004h, 05Fh, 00Bh, 0C0h, 074h, 0E7h, 0C3h
sub_2_24B5:
        call   sub_2_21FE
        mov    byte ptr [ebx + 4], al
        call   sub_2_236D
        mov    byte ptr [ebx + 5], al
        call   sub_2_238D
        or     eax, eax
        je     SHORT loc_2_24E5
        mov    dword ptr [ebx], eax
        call   sub_2_23D4
        mov    byte ptr [ebx + 7], al
        call   sub_2_237D
        mov    byte ptr [ebx + 6], al
        mov    eax, 1
loc_2_24E5:
        ret
sub_2_24E6:
        mov    dword ptr [edi + 1eh], ebx
        ret
sub_2_24EA:
        call   sub_2_2385
        test   al, 30h
        ret
sub_2_24F2:
        push   edi
        push   ebx
        call   sub_2_2402
        call   sub_2_24EA
        jne    SHORT loc_2_2531
        lea    ebx, [edi + 8]
        call   sub_2_24B5
        or     eax, eax
        jne    SHORT loc_2_250F
        inc    eax
        jmp    SHORT loc_2_2531
loc_2_250F:
        db  0BBh, 06Ch, 024h, 000h, 000h
        call   sub_2_24E6
        xor    eax, eax
        call   sub_2_2353
        mov    ax, 3e8h
        call   sub_2_240E
        call   sub_2_22D4
        xor    ax, ax
loc_2_2531:
        pop    ebx
        pop    edi
        ret
sub_2_2534:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    eax, dword ptr [edi]
        mov    eax, 1
        ja     SHORT loc_2_2548
        mov    eax, 0
loc_2_2548:
        ret
sub_2_2549:
        mov    dword ptr [edi], eax
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        add    dword ptr [edi], eax
        ret
sub_2_2554:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_2_2549
loc_2_2563:
        call   sub_2_2534
        cmp    eax, 0
        je     SHORT loc_2_2563
        pop    edi
        leave
        ret
sub_2_2570:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_2_2549
        pop    edi
        leave
        ret
sub_2_2582:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        call   sub_2_2534
        pop    edi
        leave
        ret
sub_2_2591:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        ret
sub_2_2598:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 1
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [ebp + 10h]
loc_2_25AD:
        inc    esi
        mov    eax, dword ptr [edx]
        inc    eax
        mov    dword ptr [edx], eax
        cmp    ecx, esi
        jb     SHORT loc_2_25C5
        mov    al, byte ptr [eax]
        cmp    al, 8
        je     SHORT loc_2_25C1
        cmp    al, 28h
        jne    SHORT loc_2_25AD
loc_2_25C1:
        cmp    ecx, esi
        jae    SHORT loc_2_25C9
loc_2_25C5:
        sub    eax, eax
        jmp    SHORT loc_2_260D
loc_2_25C9:
        db  0C7h, 042h, 008h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    byte ptr [edx + 0ch], 20h
        cmp    byte ptr [eax], 28h
        jne    SHORT loc_2_25E6
        db  0C7h, 042h, 008h, 000h, 000h, 000h, 000h
        mov    byte ptr [edx + 0ch], 0
loc_2_25E6:
        mov    eax, dword ptr [edx]
        mov    dword ptr [edx + 4], eax
loc_2_25EB:
        inc    esi
        mov    eax, dword ptr [edx + 4]
        inc    eax
        mov    dword ptr [edx + 4], eax
        cmp    ecx, esi
        jb     SHORT loc_2_2604
        mov    edi, dword ptr [edx + 8]
        mov    al, byte ptr [eax]
        cmp    byte ptr [edi], al
        jne    SHORT loc_2_25EB
        cmp    ecx, esi
        jae    SHORT loc_2_2608
loc_2_2604:
        sub    eax, eax
        jmp    SHORT loc_2_260D
loc_2_2608:
        mov    eax, 1
loc_2_260D:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_2611:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    edx, dword ptr [ecx]
        mov    al, byte ptr [ecx + 0ch]
        add    al, byte ptr [edx]
        inc    edx
        mov    dword ptr [ecx], edx
        ret
sub_2_2623:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        mov    cl, byte ptr [eax]
        and    ecx, 3fh
        inc    eax
        mov    dword ptr [esi], eax
        mov    al, byte ptr [eax]
        and    eax, 3fh
        shl    ecx, 6
        or     eax, ecx
        mov    ecx, 64h
        cdq
        idiv   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 4], ax
        inc    dword ptr [esi]
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_2657:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_2_2665:
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 6], al
        inc    ebx
        cmp    ebx, 3
        jb     SHORT loc_2_2665
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_2682:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
loc_2_2690:
        push   edi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [esi + ebx + 9], al
        inc    esi
        cmp    esi, 4
        jb     SHORT loc_2_2690
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_26AD:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        sub    ecx, 2
        cmp    ecx, edx
        jbe    SHORT loc_2_26D3
        mov    eax, dword ptr [eax + 8]
        mov    cl, byte ptr [edx]
        cmp    byte ptr [eax + 1], cl
        je     SHORT loc_2_26D3
        cmp    byte ptr [eax + 2], cl
        mov    eax, 0
        jne    SHORT loc_2_26D8
loc_2_26D3:
        mov    eax, 1
loc_2_26D8:
        pop    ebp
        ret
sub_2_26DA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_2_26E8:
        push   esi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_2708
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 0dh], al
        inc    ebx
        cmp    ebx, 8
        jb     SHORT loc_2_26E8
loc_2_2708:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_2712:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_2_2720:
        push   esi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_2740
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 15h], al
        inc    ebx
        cmp    ebx, 9
        jb     SHORT loc_2_2720
loc_2_2740:
        mov    byte ptr [edi + ebx + 16h], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_274F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
loc_2_2764:
        push   edi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_27B1
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi + 8]
        mov    cl, byte ptr [eax]
        cmp    byte ptr [edx + 3], cl
        jne    SHORT loc_2_2780
        inc    eax
        mov    dword ptr [edi], eax
loc_2_2780:
        sub    ebx, ebx
loc_2_2782:
        push   edi
        call   sub_2_2611
        add    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, ebx
        mov    byte ptr [esi + ecx + 1fh], al
        inc    ebx
        cmp    ebx, 7
        jb     SHORT loc_2_2782
        add    esi, 7
        inc    dword ptr [ebp - 4]
        cmp    esi, 23h
        jb     SHORT loc_2_2764
loc_2_27A5:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_27B1:
        cmp    dword ptr [ebp - 4], 4
        jae    SHORT loc_2_27A5
        mov    ecx, dword ptr [ebp - 4]
        imul   ecx, ecx, 7
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + eax + 1fh], 0
        jmp    SHORT loc_2_27A5
sub_2_27C7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_2_27D5:
        push   esi
        call   sub_2_26AD
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_27F5
        push   esi
        call   sub_2_2611
        add    esp, 4
        mov    byte ptr [edi + ebx + 42h], al
        inc    ebx
        cmp    ebx, 28h
        jb     SHORT loc_2_27D5
loc_2_27F5:
        mov    byte ptr [edi + ebx + 42h], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_2804:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_2598
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_2_2828
        sub    eax, eax
        jmp    NEAR PTR loc_2_28B7
loc_2_2828:
        push   1
        push   71h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2831:
        mov    esi, eax
        mov    eax, dword ptr [ebp + 8]
        add    esp, 8
        mov    dword ptr [eax], esi
        inc    dword ptr [ebp - 10h]
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_2623
        add    esp, 8
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_2657
        add    esp, 8
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_2682
        add    esp, 8
        mov    ecx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [eax + 1], cl
        jne    SHORT loc_2_28B2
        inc    dword ptr [ebp - 10h]
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_26DA
        add    esp, 8
        inc    dword ptr [ebp - 10h]
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_2712
        add    esp, 8
        inc    dword ptr [ebp - 10h]
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_274F
        add    esp, 8
        inc    dword ptr [ebp - 10h]
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_27C7
        add    esp, 8
loc_2_28B2:
        mov    eax, 1
loc_2_28B7:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_28BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        db  0BEh, 008h, 000h, 000h, 000h
        lea    edi, [ebp - 4]
        movsd
        mov    esi, dword ptr [ebp + 8]
        push   3
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 7fh]
        add    eax, 6
        push   eax
        call   sub_2_6FA8
        add    esp, 0ch
        cmp    eax, ebx
        jne    SHORT loc_2_2913
        mov    edi, dword ptr [esi + 7fh]
        push   10h
        lea    eax, [edi + 9]
        push   eax
        call   sub_2_71AC
        add    esp, 8
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 7fh]
        mov    ebx, 1
        mov    eax, dword ptr [eax]
        and    eax, 7fffh
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_2_291C
loc_2_2913:
        mov    eax, dword ptr [esi + 7fh]
        mov    dword ptr [eax], 0ffffffh
loc_2_291C:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2925:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_4399
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2948:
        push   ebp
        mov    ebp, esp
        push   10h
        push   2
        push   dword ptr [ebp + 8]
        call   sub_2_2EBC
        mov    esp, ebp
        pop    ebp
        ret
sub_2_295B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [ebp + 8]
        mov    edi, 1
        movzx  eax, si
        mov    dword ptr [ebp - 4], eax
        add    eax, 4
        cmp    eax, dword ptr [ebp - 4]
        jle    SHORT loc_2_29B2
loc_2_297B:
        movzx  ebx, si
        push   0
        push   0
        push   ebx
        call   sub_2_0434
        add    esp, 0ch
        push   ebx
        call   sub_2_0653
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_29A1
        or     edi, edi
        mov    edi, 1
        jne    SHORT loc_2_29A3
loc_2_29A1:
        sub    edi, edi
loc_2_29A3:
        inc    si
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, si
        add    eax, 4
        cmp    ecx, eax
        jl     SHORT loc_2_297B
loc_2_29B2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_29BB:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 4]
        call   sub_2_0458
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 4]
        call   sub_2_0488
        add    esp, 4
        push   esi
        call   sub_2_07B4
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        mov    ax, word ptr [eax + 0ah]
        push   eax
        call   sub_2_295B
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 0ah
        push   eax
        call   sub_2_2948
        add    esp, 4
        lea    eax, [esi + 0afh]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_2_2A3B:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   91h
        push   3
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 0ah
        push   eax
        call   sub_2_2EBC
        add    esp, 0ch
        push   esi
        call   sub_2_07DB
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 0ah
        push   eax
        call   sub_2_2B2A
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_2A7E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   2
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, 0deh
        je     SHORT loc_2_2AA6
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, 0deh
        je     SHORT loc_2_2AA6
        jmp    SHORT loc_2_2AD6
loc_2_2AA6:
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        mov    bl, al
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, bl
        jne    SHORT loc_2_2AD6
        push   2
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, bl
        mov    eax, 1
        je     SHORT loc_2_2AD8
loc_2_2AD6:
        sub    eax, eax
loc_2_2AD8:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_2ADC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   91h
        push   3
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2EBC
        add    esp, 0ch
        push   0a5h
        push   1
        push   esi
        call   sub_2_2EBC
        add    esp, 0ch
        push   1eh
        lea    eax, [esi + 2]
        push   eax
        call   sub_2_2554
        add    esp, 8
        push   1
        push   esi
        call   sub_2_2ED2
        add    esp, 8
        cmp    al, 0a5h
        mov    eax, 1
        je     SHORT loc_2_2B27
        sub    eax, eax
loc_2_2B27:
        pop    esi
        pop    ebp
        ret
sub_2_2B2A:
        push   ebp
        mov    ebp, esp
        push   0
        push   2
        push   dword ptr [ebp + 8]
        call   sub_2_2EBC
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2B3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        lea    esi, [edi + 2]
        push   0fah
        push   esi
        call   sub_2_2570
        add    esp, 8
        push   2
        push   edi
        call   sub_2_2ED2
        add    esp, 8
        and    al, byte ptr [ebp + 0ch]
        mov    bl, al
loc_2_2B6E:
        push   2
        push   edi
        call   sub_2_2ED2
        add    esp, 8
        and    al, byte ptr [ebp + 0ch]
        xor    bl, al
        or     byte ptr [ebp - 1], bl
        mov    bl, al
        push   esi
        call   sub_2_2582
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_2B6E
        mov    al, byte ptr [ebp - 1]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2B9A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2A7E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_2C0E
        push   esi
        call   sub_2_2ADC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_2C0E
        mov    edi, 5
loc_2_2BC2:
        push   esi
        call   sub_2_2948
        add    esp, 4
        push   0ffh
        push   esi
        call   sub_2_2B3D
        add    esp, 8
        mov    bl, al
        push   esi
        call   sub_2_2B2A
        add    esp, 4
        not    bl
        push   ebx
        push   esi
        call   sub_2_2B3D
        add    esp, 8
        mov    bl, al
        push   esi
        call   sub_2_2948
        add    esp, 4
        push   ebx
        push   esi
        call   sub_2_2EE5
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_2C10
        dec    edi
        jne    SHORT loc_2_2BC2
        jmp    SHORT loc_2_2C10
loc_2_2C0E:
        sub    eax, eax
loc_2_2C10:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_2C15:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    edi, [esi + 0afh]
        push   edi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_2C4E
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 0cfh], eax
        db  0C7h, 086h, 0D3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_2_2C50
loc_2_2C4E:
        sub    eax, eax
loc_2_2C50:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_2C54:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 8], 2
        jne    SHORT loc_2_2C64
        mov    byte ptr [eax + 8], 9
loc_2_2C64:
        pop    ebp
        ret
sub_2_2C66:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    esi, eax
        mov    edi, dword ptr [ecx + 0d7h]
        add    edi, 0ah
        movsd
        movsd
        movsb
        mov    ecx, dword ptr [ecx + 0d7h]
        mov    ax, word ptr [eax]
        mov    word ptr [ecx + 8], ax
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_2C95:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 28h]
        push   esi
        call   sub_2_409F
        add    esp, 4
        mov    eax, dword ptr [edi + 0d7h]
        push   1
        movzx  eax, byte ptr [eax + 12h]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 10h], 1
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 30h], eax
        mov    byte ptr [esi + 5ah], 0
        mov    word ptr [esi + 52h], 0
        mov    dword ptr [esi], 1
        mov    eax, 1
        mov    dword ptr [esi + 4], 1
        mov    dword ptr [esi + 40h], 3ch
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_2CF4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d7h]
        movzx  esi, word ptr [eax + 8]
        lea    edi, [esi + 4]
        cmp    esi, edi
        jae    SHORT loc_2_2D31
loc_2_2D0D:
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_2_0633
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_2D2C
        push   0
        push   0
        push   esi
        call   sub_2_0668
        add    esp, 0ch
loc_2_2D2C:
        inc    esi
        cmp    esi, edi
        jb     SHORT loc_2_2D0D
loc_2_2D31:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_2D3A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_2_2C54
        add    esp, 4
        push   13h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_2_2DC6
        push   esi
        push   ebx
        call   sub_2_2C66
        add    esp, 8
        push   esi
        call   sub_2_2C15
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_2DC6
        mov    ebx, dword ptr [esi + 0d7h]
        push   edi
        push   dword ptr [esi + 0cfh]
        mov    al, byte ptr [ebx + 12h]
        push   eax
        call   sub_2_0721
        add    esp, 0ch
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0d7h]
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_2_2DB7
        push   esi
        call   sub_2_2C95
        add    esp, 4
        push   esi
        call   sub_2_2CF4
        add    esp, 4
        mov    edi, 1
        jmp    SHORT loc_2_2DC6
loc_2_2DB7:
        add    esi, 0afh
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_2DC6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_2DCD:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 4]
        call   sub_2_0467
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 0ah
        push   eax
        call   sub_2_2B2A
        add    esp, 4
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
loc_2_2DFF:
        sub    eax, eax
        ret
loc_2_2E02:
        sub    eax, eax
        ret
sub_2_2E05:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 018h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 020h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
loc_2_2E2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        sub    esi, esi
        mov    word ptr [ebp - 0ch], 23ch
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2B9A
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_2EB3
        push   dword ptr [ebp + 0ch]
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_2D3A
        add    esp, 0ch
        cmp    eax, esi
        je     SHORT loc_2_2EA3
        push   edi
        call   sub_2_2E05
        add    esp, 4
        push   edi
        call   sub_2_4333
        add    esp, 4
        push   edi
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [edi + 2ch]
        push   edi
        call   sub_2_9371
        add    esp, 8
        push   edi
        call   sub_2_07DB
        add    esp, 4
        push   edi
        call   sub_2_2DCD
        add    esp, 4
        mov    esi, 1
loc_2_2EA3:
        or     esi, esi
        jne    SHORT loc_2_2EB3
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_2948
        add    esp, 4
loc_2_2EB3:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_2EBC:
        enter  0, 0
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    dx, word ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        pop    edi
        leave
        ret
sub_2_2ED2:
        enter  0, 0
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dx, word ptr [edi]
        add    dx, word ptr [ebp + 0ch]
        in     al, dx
        pop    edi
        leave
        ret
sub_2_2EE5:
        enter  0, 0
        push   edi
        push   ebx
        xor    eax, eax
        movzx  ebx, byte ptr [ebp + 0ch]
        cmp    ebx, 8
        ja     SHORT loc_2_2F06
        db  08Ah, 083h, 068h, 001h, 000h, 000h
        or     al, al
        je     SHORT loc_2_2F06
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 8], al
loc_2_2F06:
        pop    ebx
        pop    edi
        leave
        ret
        db  000h, 000h
loc_2_2F0C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2F16:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, eax
        mov    eax, dword ptr [esi + 0d7h]
        push   dword ptr [eax + 0ch]
        call   sub_2_0488
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 10h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_39A7
        add    esp, 0ch
        call   sub_2_374C
        push   edi
        call   sub_2_3868
        add    esp, 4
        call   sub_2_3840
        push   esi
        call   sub_2_07B4
        add    esp, 4
        lea    eax, [esi + 0afh]
        push   eax
        call   sub_2_080D
        add    esp, 4
        push   esi
        call   sub_2_4362
        add    esp, 4
        push   esi
        call   sub_2_44A3
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_2F8E:
        push   ebp
        mov    ebp, esp
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_2F96:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_3208
        mov    esp, ebp
        pop    ebp
        ret
sub_2_2FA3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        mov    al, cl
        and    al, 10h
        sub    al, 10h
        cmp    al, 1
        mov    edx, dword ptr [ebp + 8]
        sbb    eax, eax
        and    eax, 1
        inc    eax
        mov    word ptr [edx + 69h], ax
        mov    al, cl
        and    al, 20h
        sub    al, 20h
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 73h], eax
        mov    al, cl
        and    al, 80h
        sub    al, 80h
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 6bh], eax
        and    cl, 40h
        sub    cl, 40h
        cmp    cl, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 6fh], eax
        mov    eax, 1
        pop    ebp
        ret
sub_2_2FF5:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ecx]
        and    al, 30h
        shr    al, 4
        movzx  dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    word ptr [eax + 65h], dx
        mov    cl, byte ptr [ecx]
        shr    cl, 6
        push   ecx
        call   sub_2_3FD6
        add    esp, 8
        mov    eax, 1
        pop    ebp
        ret
sub_2_3023:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    al, byte ptr [edi]
        cmp    byte ptr [edi + 1], 1
        jne    SHORT loc_2_3064
        mov    esi, dword ptr [ebp + 8]
        and    ax, 7
        mov    word ptr [esi + 67h], ax
        push   esi
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        push   edi
        push   esi
        jne    SHORT loc_2_3055
        call   sub_2_2FA3
        jmp    SHORT loc_2_305A
loc_2_3055:
        call   sub_2_2FF5
loc_2_305A:
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_2_3066
loc_2_3064:
        sub    eax, eax
loc_2_3066:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_306A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    al, byte ptr [esi + 1]
        or     al, al
        je     SHORT loc_2_30C0
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 5ah], al
        mov    cl, byte ptr [esi]
        mov    al, cl
        and    ax, 0fh
        shl    ax, 3
        and    cl, 70h
        shr    cl, 4
        movzx  cx, cl
        or     ax, cx
        mov    word ptr [edx + 52h], ax
        movzx  ax, byte ptr [esi + 2]
        mov    word ptr [edx + 50h], ax
        mov    cl, byte ptr [esi]
        shr    cl, 7
        mov    eax, dword ptr [edx + 7bh]
        movzx  ecx, cl
        xor    ecx, eax
        and    ecx, 1
        xor    ecx, eax
        mov    dword ptr [edx + 7bh], ecx
        mov    eax, 1
        jmp    SHORT loc_2_30C2
loc_2_30C0:
        sub    eax, eax
loc_2_30C2:
        pop    esi
        pop    ebp
        ret
loc_2_30C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_30D3:
        mov    edi, dword ptr [ebp + 8]
        mov    esi, eax
        lea    ebx, [edi + 28h]
        push   ebx
        call   sub_2_3F1C
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_30EF
        sub    eax, eax
        jmp    NEAR PTR loc_2_3201
loc_2_30EF:
        push   ebx
        call   sub_2_3F69
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_3101
        jmp    NEAR PTR loc_2_31FC
loc_2_3101:
        call   sub_2_374C
        push   esi
        push   0
        call   sub_2_38A3
        add    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_37A4
        add    esp, 8
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        call   sub_2_3023
        add    esp, 8
        push   ebx
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        jne    NEAR PTR loc_2_31CF
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_306A
        add    esp, 8
        movzx  eax, word ptr [ebx + 52h]
        push   eax
        push   ebx
        call   sub_2_4026
        add    esp, 8
        cmp    word ptr [ebx + 69h], 2
        jne    SHORT loc_2_31CF
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        cmp    dword ptr [ebx + 83h], 0
        jne    SHORT loc_2_31A4
        push   1
        push   87h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_318E:
        add    esp, 8
        mov    dword ptr [ebx + 83h], eax
        or     eax, eax
        je     SHORT loc_2_31A4
        push   eax
        call   sub_2_409F
        add    esp, 4
loc_2_31A4:
        mov    eax, dword ptr [ebx + 83h]
        or     eax, eax
        je     SHORT loc_2_31CF
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [ebx + 83h]
        movzx  ecx, word ptr [eax + 52h]
        push   ecx
        push   eax
        call   sub_2_4026
        add    esp, 8
loc_2_31CF:
        push   esi
        push   3
        call   sub_2_38A3
        add    esp, 8
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        push   edi
        call   sub_2_07DB
        add    esp, 4
        push   edi
        call   sub_2_3448
        add    esp, 4
        call   sub_2_3840
loc_2_31FC:
        mov    eax, 1
loc_2_3201:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_3208:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        call   sub_2_374C
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   3
        call   sub_2_32DC
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_2_3251
        push   esi
        call   sub_2_35EC
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_3251
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_07DB
        add    esp, 4
        push   esi
        call   sub_2_35CE
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_3251
        mov    edi, 1
loc_2_3251:
        call   sub_2_3840
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_325C:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    esi, esi
        db  089h, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [ebp + 8], 8
        jne    SHORT loc_2_3282
        push   dword ptr [ebp + 0ch]
        call   sub_2_3E9C
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_3282
        mov    esi, 1
loc_2_3282:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
sub_2_3287:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_325C
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_32B1
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 1
        mov    eax, 1
        jmp    SHORT loc_2_32D7
loc_2_32B1:
        test   byte ptr [ebp + 8], 4
        je     SHORT loc_2_32C1
        mov    eax, dword ptr [esi + 4]
        or     byte ptr [eax + 410h], 4
loc_2_32C1:
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        push   11h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_32CD:
        add    esp, 0ch
        mov    ax, word ptr [ebp - 4]
        and    eax, 4
loc_2_32D7:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_32DC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_38CB
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_3318
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_387B
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_3318
        push   0ch
        call   sub_2_0476
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_2_331A
loc_2_3318:
        sub    eax, eax
loc_2_331A:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_331F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   0
        call   sub_2_38A3
        mov    esi, dword ptr [ebp + 8]
        add    esp, 8
        lea    eax, [esi + 54h]
        push   eax
        push   0
        push   0
        push   4
        call   sub_2_3DEC
        add    esp, 10h
        mov    dword ptr [esi + 56h], eax
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_3418
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_2_306A
        add    esp, 8
        movzx  eax, word ptr [esi + 52h]
        push   eax
        push   esi
        call   sub_2_4026
        add    esp, 8
        test   byte ptr [esi + 7bh], 1
        je     NEAR PTR loc_2_3418
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_37A4
        add    esp, 8
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_2_3023
        add    esp, 8
        push   esi
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 2
        jne    SHORT loc_2_3418
        cmp    word ptr [esi + 69h], 2
        jne    SHORT loc_2_3418
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_37F2
        add    esp, 8
        push   1
        push   87h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_33DC:
        add    esp, 8
        mov    dword ptr [esi + 83h], eax
        or     eax, eax
        je     SHORT loc_2_3418
        push   eax
        call   sub_2_409F
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [esi + 83h]
        call   sub_2_306A
        add    esp, 8
        mov    eax, dword ptr [esi + 83h]
        movzx  ecx, word ptr [eax + 52h]
        push   ecx
        push   eax
        call   sub_2_4026
        add    esp, 8
loc_2_3418:
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_2_3429:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_409F
        add    esp, 4
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_331F
        add    esp, 8
        pop    esi
        pop    ebp
        ret
sub_2_3448:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 0d7h]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [eax + 0d7h]
        mov    dword ptr [eax + 4], 0
        ret
sub_2_3469:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_3448
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        mov    byte ptr [eax + 14h], 0ch
        mov    eax, dword ptr [esi + 0d7h]
        and    dword ptr [eax + 10h], 0fffffffeh
        mov    eax, dword ptr [esi + 0d7h]
        pop    esi
        and    dword ptr [eax + 10h], 0fffffffdh
        mov    eax, 1
        pop    ebp
        ret
sub_2_349F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        lea    edi, [esi + 0afh]
        push   edi
        call   sub_2_082F
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_34D8
        mov    dword ptr [edi], esi
        mov    dword ptr [esi + 0cfh], eax
        db  0C7h, 086h, 0D3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_2_34DA
loc_2_34D8:
        sub    eax, eax
loc_2_34DA:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_34DE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        push   4
        mov    eax, dword ptr [ecx + 0d7h]
        lea    esi, [ecx + 28h]
        movzx  eax, byte ptr [eax + 14h]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [ecx + 38h], 1
        mov    dword ptr [esi], 4
        call   sub_2_05ED
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [esi + 30h], eax
        mov    dword ptr [esi + 40h], 28h
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_3521:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        push   15h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_452A
        add    esp, 8
        cmp    eax, edi
        je     NEAR PTR loc_2_35C7
        push   esi
        call   sub_2_3469
        add    esp, 4
        push   esi
        call   sub_2_349F
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_35C7
        push   dword ptr [ebp + 0ch]
        call   sub_2_3855
        add    esp, 4
        mov    ebx, dword ptr [esi + 0d7h]
        push   edi
        push   dword ptr [esi + 0cfh]
        mov    al, byte ptr [ebx + 14h]
        push   eax
        call   sub_2_0721
        add    esp, 0ch
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [esi + 0d7h]
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_2_35B8
        call   sub_2_3840
        push   esi
        call   sub_2_34DE
        add    esp, 4
        mov    eax, dword ptr [esi + 0d7h]
        add    eax, 10h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_3925
        add    esp, 0ch
        mov    edi, 1
        jmp    SHORT loc_2_35C7
loc_2_35B8:
        add    esi, 0afh
        push   esi
        call   sub_2_080D
        add    esp, 4
loc_2_35C7:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_35CE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d7h]
        push   dword ptr [eax + 0ch]
        call   sub_2_0467
        add    esp, 4
        mov    eax, 1
        pop    ebp
        ret
sub_2_35EC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   3
        call   sub_2_38A3
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_3627
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_3627
        push   esi
        push   3
        push   2
        call   sub_2_388F
        add    esp, 0ch
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_2_3629
loc_2_3627:
        sub    eax, eax
loc_2_3629:
        pop    esi
        pop    ebp
        ret
sub_2_362C:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_35EC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_3658
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_38DF
        add    esp, 8
        cmp    eax, 2
        mov    eax, 1
        je     SHORT loc_2_365A
loc_2_3658:
        sub    eax, eax
loc_2_365A:
        pop    esi
        pop    ebp
        ret
loc_2_365D:
        sub    eax, eax
        ret
sub_2_3660:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        db  0C7h, 040h, 014h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 018h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 020h, 000h, 000h, 000h, 000h
        mov    eax, 1
        ret
loc_2_3689:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        sub    edi, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_36A0:
        mov    esi, eax
        call   sub_2_374C
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        push   dword ptr [eax + 8]
        call   sub_2_3287
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_2_3739
        push   esi
        push   3
        call   sub_2_32DC
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_2_3739
        mov    ebx, dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        lea    eax, [ebx + 28h]
        push   esi
        push   eax
        call   sub_2_3429
        add    esp, 0ch
        push   esi
        call   sub_2_362C
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_3739
        push   esi
        push   ebx
        call   sub_2_3521
        add    esp, 8
        cmp    eax, edi
        je     SHORT loc_2_3739
        push   ebx
        call   sub_2_3660
        add    esp, 4
        push   ebx
        call   sub_2_4333
        add    esp, 4
        push   ebx
        call   sub_2_A04C
        add    esp, 4
        push   dword ptr [ebx + 2ch]
        push   ebx
        call   sub_2_9371
        add    esp, 8
        push   ebx
        call   sub_2_07DB
        add    esp, 4
        push   ebx
        call   sub_2_35CE
        add    esp, 4
        mov    edi, 1
loc_2_3739:
        or     edi, edi
        jne    SHORT loc_2_3742
        call   sub_2_3840
loc_2_3742:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_374C:
        push   esi
        call   sub_2_0517
        call   sub_2_3A27
        call   sub_2_0553
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_3769
        call   sub_2_059E
        jmp    SHORT loc_2_376E
loc_2_3769:
        call   sub_2_0535
loc_2_376E:
        mov    eax, esi
        pop    esi
        ret
sub_2_3772:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_2_378C
        push   dword ptr [ebp + 0ch]
        push   eax
        push   6
        call   sub_2_3A71
        add    esp, 0ch
        jmp    SHORT loc_2_378E
loc_2_378C:
        sub    eax, eax
loc_2_378E:
        pop    ebp
        ret
sub_2_3790:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   6
        call   sub_2_3A32
        mov    esp, ebp
        pop    ebp
        ret
sub_2_37A4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   2
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_37ED
        push   esi
        push   2
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_37ED
        push   esi
        push   2
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_37ED
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_3790
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_2_37EF
loc_2_37ED:
        sub    eax, eax
loc_2_37EF:
        pop    esi
        pop    ebp
        ret
sub_2_37F2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_383B
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_383B
        push   esi
        push   1
        call   sub_2_3772
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_383B
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_2_3790
        add    esp, 8
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_2_383D
loc_2_383B:
        sub    eax, eax
loc_2_383D:
        pop    esi
        pop    ebp
        ret
sub_2_3840:
        call   sub_2_05A5
        call   sub_2_0574
        call   sub_2_0535
        mov    eax, 1
        ret
sub_2_3855:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   1
        push   0
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret
sub_2_3868:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        push   0
        push   0
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret
sub_2_387B:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   1
        call   sub_2_3A32
        mov    esp, ebp
        pop    ebp
        ret
sub_2_388F:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        push   2
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret
sub_2_38A3:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   3
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   4
        call   sub_2_3A32
        mov    esp, ebp
        pop    ebp
        ret
sub_2_38CB:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   5
        call   sub_2_3A71
        mov    esp, ebp
        pop    ebp
        ret
sub_2_38DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_068C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_391C
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 4]
        push   7
        call   sub_2_3A71
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_3915
        mov    eax, 2
        jmp    SHORT loc_2_391E
loc_2_3915:
        mov    eax, 1
        jmp    SHORT loc_2_391E
loc_2_391C:
        sub    eax, eax
loc_2_391E:
        mov    esp, ebp
        pop    ebp
        ret
        db  02Bh, 0C0h, 0C3h
sub_2_3925:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   dword ptr [ebp + 8]
        push   15h
        call   sub_2_06CE
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_39A0
        mov    esi, dword ptr [ebp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        or     dword ptr [esi], 1
        push   0
        push   dword ptr [ebp + 0ch]
        call   sub_2_06A8
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_398F
        db  068h, 000h, 000h, 000h, 000h
        push   15h
        call   sub_2_06EB
        add    esp, 8
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 10h
        mov    word ptr [ebp - 8], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 6], eax
        lea    eax, [ebp - 8]
        push   eax
        push   15h
        call   sub_2_0709
        add    esp, 8
        or     dword ptr [esi], 2
loc_2_398F:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_2_39A2
loc_2_39A0:
        sub    eax, eax
loc_2_39A2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_39A7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 10h]
        test   byte ptr [esi], 1
        je     SHORT loc_2_39C7
        push   dword ptr [ebp + 8]
        push   15h
        call   sub_2_05AC
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_39C7
        and    dword ptr [esi], 0fffffffeh
loc_2_39C7:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_2_39DC:
        db  083h, 03Dh, 0F8h, 013h, 000h, 000h, 000h
        db  00Fh, 085h, 0F4h, 0CBh, 0FDh, 0FFh
        movzx  eax, al
        cmp    eax, 7
        ja     SHORT loc_2_39F3
        jmp    SHORT loc_2_3A09
loc_2_39F3:
        or     word ptr [ebp + 2ch], 1
        mov    byte ptr [ebp + 1dh], 1
        clc
        ret
loc_2_39FE:
        and    word ptr [ebp + 2ch], 0fffeh
        mov    byte ptr [ebp + 1dh], 0
        clc
        ret
loc_2_3A09:
        cmp    eax, 7
        je     SHORT loc_2_39FE
        db  08Ah, 080h, 0FCh, 013h, 000h, 000h
        mov    byte ptr [ebp + 1dh], al
        or     word ptr [ebp + 2ch], 1
        clc
        ret
        db  08Bh, 0C0h
loc_2_3A20:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
sub_2_3A27:
        db  0C7h, 005h, 0ACh, 001h, 000h, 000h, 014h, 000h, 000h, 000h
        ret
sub_2_3A32:
        enter  0, 0
        push   ebx
        push   esi
        push   edi
        mov    al, byte ptr [ebp + 8]
        mov    ebx, 0
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [esi + 8]
        call   sub_2_3A98
        mov    eax, 0
        jb     SHORT loc_2_3A6C
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [edi], al
        mov    al, byte ptr [esi + 18h]
        mov    byte ptr [edi + 1], al
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [edi + 2], al
        mov    eax, 1
loc_2_3A6C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_2_3A71:
        enter  0, 0
        push   esi
        push   ebx
        mov    al, byte ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [esi + 8]
        call   sub_2_3A98
        mov    eax, 1
        jae    SHORT loc_2_3A94
        mov    eax, 0
loc_2_3A94:
        pop    ebx
        pop    esi
        leave
        ret
sub_2_3A98:
        db  0FFh, 005h, 0F8h, 013h, 000h, 000h
loc_2_3A9E:
        mov    byte ptr [esi + 1ch], al
        mov    byte ptr [esi + 1dh], 0c2h
        cmp    al, 7
        jne    SHORT loc_2_3AB6
        mov    word ptr [esi + 10h], bx
        shr    ebx, 10h
        mov    word ptr [esi + 38h], bx
        jmp    SHORT loc_2_3AB9
loc_2_3AB6:
        mov    byte ptr [esi + 11h], bl
loc_2_3AB9:
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     dword ptr [esi + 2ch], 0
        jae    SHORT loc_2_3ADA
        db  0FFh, 00Dh, 0ACh, 001h, 000h, 000h
        jle    SHORT loc_2_3AD9
        cmp    byte ptr [esi + 1dh], 4
        je     SHORT loc_2_3A9E
loc_2_3AD9:
        stc
loc_2_3ADA:
        db  0FFh, 00Dh, 0F8h, 013h, 000h, 000h
        ret
        db  000h, 000h, 000h
sub_2_3AE4:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        sub    esi, esi
        call   sub_2_3B43
        cmp    eax, esi
        je     SHORT loc_2_3B3A
        mov    word ptr [ebp - 4], 0e800h
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        push   15h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_3B08:
        add    esp, 0ch
        test   al, 1
        jne    SHORT loc_2_3B3A
        db  066h, 039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_3B30
        db  0B8h, 000h, 000h, 000h, 000h
loc_2_3B1D:
        mov    cx, word ptr [ebp - 10h]
        cmp    word ptr [eax], cx
        je     SHORT loc_2_3B30
        add    eax, 4
        inc    esi
        cmp    word ptr [eax], 0
        jne    SHORT loc_2_3B1D
loc_2_3B30:
        db  066h, 08Bh, 004h, 0B5h, 002h, 000h, 000h, 000h
        jmp    SHORT loc_2_3B3E
loc_2_3B3A:
        mov    ax, 101h
loc_2_3B3E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_3B43:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        db  0BEh, 044h, 000h, 000h, 000h
        lea    edi, [ebp - 8]
        movsd
        movsw
        movsb
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_3B5C:
        mov    edx, eax
        lea    edi, [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 0fffeah
        push   eax
        call   sub_2_6FA8
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_3B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        push   edi
        call   sub_2_3B43
        or     eax, eax
        je     SHORT loc_2_3BAC
        cmp    dword ptr [ebp + 8], 1
        mov    edi, 1
        je     SHORT loc_2_3BAE
loc_2_3BAC:
        sub    edi, edi
loc_2_3BAE:
        or     edi, edi
        je     NEAR PTR loc_2_3C60
        mov    word ptr [ebp - 4], 0e802h
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        push   15h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_3BC8:
        add    esp, 0ch
        and    eax, 1
        cmp    eax, 1
        sbb    edi, edi
        neg    edi
        or     edi, edi
        je     NEAR PTR loc_2_3C60
        mov    di, word ptr [ebp - 10h]
        and    edi, 1
        je     SHORT loc_2_3C60
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 4], 1
        mov    dword ptr [esi + 8], 1
        call   sub_2_3AE4
        movzx  eax, ax
        mov    dword ptr [esi + 10h], eax
        mov    ax, word ptr [ebp - 10h]
        and    ax, 4
        shr    ax, 2
        movzx  eax, ax
        mov    dword ptr [esi + 0ch], eax
        test   byte ptr [ebp - 10h], 2
        je     SHORT loc_2_3C22
        mov    dword ptr [esi], 4
        jmp    SHORT loc_2_3C60
loc_2_3C22:
        mov    dword ptr [esi], 2
        mov    ax, word ptr [ebp - 10h]
        and    ax, 0f0h
        shr    ax, 4
        movzx  eax, ax
        mov    dword ptr [esi + 14h], eax
        mov    ax, word ptr [ebp - 10h]
        and    ax, 300h
        shr    ax, 8
        movzx  eax, ax
        inc    eax
        mov    dword ptr [esi + 18h], eax
        cmp    eax, 2
        jbe    SHORT loc_2_3C60
        cmp    word ptr [ebp - 0ch], 0
        je     SHORT loc_2_3C60
        movzx  eax, word ptr [ebp - 0ch]
        mov    dword ptr [esi + 1ch], eax
loc_2_3C60:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_3C68:
        call   sub_2_3B43
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
sub_2_3C74:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 8]
        movsd
        movsd
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_3C8B:
        mov    edx, eax
        lea    edi, [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 0fe010h
        push   eax
        call   sub_2_6FA8
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        db  0BEh, 008h, 000h, 000h, 000h
        lea    edi, [ebp - 4]
        movsw
        movsb
        call   sub_2_3C74
        or     eax, eax
        je     SHORT loc_2_3D0E
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_3CE0:
        mov    edx, eax
        lea    edi, [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 0fe000h
        push   eax
        call   sub_2_6FA8
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_3D0E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_3D14:
        push   ebp
        mov    ebp, esp
        call   sub_2_3C74
        or     eax, eax
        je     SHORT loc_2_3D3A
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   15h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_3D2D:
        add    esp, 0ch
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_3D3A:
        pop    ebp
        ret
loc_2_3D3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        sub    esi, esi
        mov    word ptr [ebp - 4], 0c280h
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        call   sub_2_3D14
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_2_3D60
        movzx  esi, word ptr [ebp - 10h]
loc_2_3D60:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_3D67:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        mov    word ptr [ebp - 4], 0c281h
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 10h], ax
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        call   sub_2_3D14
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_3DE5
        mov    cl, byte ptr [ebp - 4]
        and    cl, 38h
        shr    cl, 3
        mov    edx, dword ptr [ebp + 0ch]
        movzx  ecx, cl
        db  08Bh, 00Ch, 08Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [edx], ecx
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 1
        mov    dword ptr [edx + 4], ecx
        mov    cl, byte ptr [ebp - 4]
        and    cl, 4
        shr    cl, 2
        movzx  ecx, cl
        mov    dword ptr [edx + 0ch], ecx
        mov    cl, byte ptr [ebp - 4]
        and    cl, 2
        shr    cl, 1
        movzx  ecx, cl
        mov    dword ptr [edx + 8], ecx
        movzx  ecx, word ptr [ebp - 10h]
        mov    dword ptr [edx + 10h], ecx
        movzx  ecx, byte ptr [ebp - 8]
        mov    dword ptr [edx + 14h], ecx
        movzx  ecx, byte ptr [ebp - 7]
        mov    dword ptr [edx + 18h], ecx
        movzx  ecx, word ptr [ebp - 0ch]
        mov    dword ptr [edx + 1ch], ecx
loc_2_3DE5:
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_3DEC:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        sub    ebx, ebx
        mov    word ptr [eax], bx
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_2_3E92
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_2_3E13:
        or     ebx, ebx
        jne    SHORT loc_2_3E92
        mov    eax, dword ptr [ebp - 4]
        call   dword ptr [eax]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_3E86
loc_2_3E22:
        or     ebx, ebx
        jne    SHORT loc_2_3E86
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   edi
        call   dword ptr [eax + 4]
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_3E83
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_2_3E54
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_2_3E54
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_2_3E54
        cmp    dword ptr [ebp - 24h], esi
        mov    ebx, 1
        je     SHORT loc_2_3E56
loc_2_3E54:
        sub    ebx, ebx
loc_2_3E56:
        cmp    esi, 2
        jne    SHORT loc_2_3E76
        or     ebx, ebx
        je     SHORT loc_2_3E74
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 0ch], eax
        jne    SHORT loc_2_3E74
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 10h], eax
        mov    ebx, 1
        je     SHORT loc_2_3E76
loc_2_3E74:
        sub    ebx, ebx
loc_2_3E76:
        or     ebx, ebx
        je     SHORT loc_2_3E83
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [eax], cx
loc_2_3E83:
        dec    edi
        jne    SHORT loc_2_3E22
loc_2_3E86:
        add    dword ptr [ebp - 4], 8
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_3E13
loc_2_3E92:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_3E9C:
        enter  0, 0
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    esi, dword ptr [esi + 8]
        mov    word ptr [esi + 1ch], 0f963h
        mov    word ptr [esi + 10h], 4954h
        mov    word ptr [esi + 14h], 0
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     dword ptr [esi + 2ch], 0
        jb     SHORT loc_2_3F14
        mov    word ptr [esi + 1ch], 0f95fh
        mov    eax, 15h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        bt     dword ptr [esi + 2ch], 0
        jb     SHORT loc_2_3F03
        xor    ebx, ebx
        xor    eax, eax
        mov    bx, word ptr [esi + 38h]
        shl    ebx, 4
        mov    ax, word ptr [esi + 10h]
        add    ebx, eax
        cmp    word ptr [ebx], 0ah
        jl     SHORT loc_2_3F14
        test   word ptr [ebx + 14h], 40h
        je     SHORT loc_2_3F14
loc_2_3F03:
        db  0C7h, 005h, 0F4h, 013h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, 1
        jmp    SHORT loc_2_3F16
loc_2_3F14:
        xor    eax, eax
loc_2_3F16:
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_2_3F1C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 22h
        je     SHORT loc_2_3F44
        cmp    eax, 23h
        je     SHORT loc_2_3F44
        cmp    eax, 18h
        je     SHORT loc_2_3F3D
        cmp    eax, 19h
        je     SHORT loc_2_3F3D
        sub    eax, eax
        jmp    SHORT loc_2_3F49
loc_2_3F3D:
        mov    eax, 1
        jmp    SHORT loc_2_3F49
loc_2_3F44:
        mov    eax, 2
loc_2_3F49:
        pop    ebp
        ret
sub_2_3F4B:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7fh]
        or     eax, eax
        je     SHORT loc_2_3F65
        cmp    dword ptr [eax], 0ffffffh
        mov    eax, 1
        jne    SHORT loc_2_3F67
loc_2_3F65:
        sub    eax, eax
loc_2_3F67:
        pop    ebp
        ret
_PTEXT ends
        end
