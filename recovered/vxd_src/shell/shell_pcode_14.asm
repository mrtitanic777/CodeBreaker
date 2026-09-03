; shell_pcode_14.asm -- SHELL object 14 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 08Bh, 00Dh, 000h, 00Ah, 000h, 000h, 0F6h, 004h, 019h, 008h, 075h, 015h, 0F6h, 044h, 019h
        db  00Eh, 008h, 00Fh, 084h, 095h, 000h, 000h, 000h, 0F6h, 004h, 019h, 010h, 00Fh, 084h, 08Bh, 000h
        db  000h, 000h, 080h, 04Ch, 019h, 00Eh, 008h, 068h, 002h, 000h, 008h, 000h, 068h, 000h, 000h, 000h
        db  000h, 0CDh, 020h, 059h, 001h, 001h, 000h, 092h, 0B4h, 009h, 06Ah, 021h, 0CDh, 020h, 08Fh, 000h
        db  001h, 000h, 003h, 024h, 024h, 083h, 0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h, 0CDh, 020h, 01Ah, 000h, 001h, 000h, 0C6h, 045h, 01Dh, 001h
        db  0B8h, 016h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0F7h, 045h, 02Ch, 040h, 000h
        db  000h, 000h, 074h, 013h, 0B8h, 028h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0CDh
        db  020h, 038h, 000h, 001h, 000h, 0EBh, 0D5h, 033h, 0C0h, 089h, 045h, 01Ch, 0B0h, 016h, 0CDh, 020h
        db  084h, 000h, 001h, 000h, 080h, 07Dh, 01Ch, 003h, 075h, 0C2h, 0CDh, 020h, 086h, 000h, 001h, 000h
        db  08Bh, 0F4h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 083h, 0C4h, 06Ch, 0EBh, 016h, 0F6h, 004h, 019h
        db  020h, 074h, 00Bh, 0F6h, 044h, 019h, 00Fh, 040h, 00Fh, 084h, 064h, 0FFh, 0FFh, 0FFh, 080h, 064h
        db  019h, 00Eh, 0F7h, 061h, 0C3h, 000h, 000h, 000h
loc_14_00C8:
        push   edx
        push   ebx
        mov    ebp, esp
        db  08Bh, 035h, 000h, 00Ah, 000h, 000h
        test   dl, 1
        je     SHORT loc_14_00DC
        mov    byte ptr [esi + ebx + 0bh], 83h
loc_14_00DC:
        db  0CDh, 020h, 030h, 000h, 001h, 000h
        cmp    dword ptr [ebp], ebx
        jne    SHORT loc_14_00F9
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        inc    eax
        push   eax
        push   ebx
        db  0E8h, 0B0h, 000h, 0F2h, 0FFh
        pop    ecx
        pop    ecx
loc_14_00F9:
        mov    ebx, dword ptr [ebp]
        xor    eax, eax
        xor    edx, edx
        test   byte ptr [ebp + 4], 4
        jne    SHORT loc_14_013A
        test   byte ptr [ebp + 4], 18h
        je     SHORT loc_14_011D
        xor    ecx, ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        jne    SHORT loc_14_013A
        mov    eax, 8
        jmp    SHORT loc_14_0138
loc_14_011D:
        test   byte ptr [esi + ebx], 2
        jne    SHORT loc_14_0133
        test   byte ptr [ebp + 4], 1
        je     SHORT loc_14_0138
        xor    ecx, ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        jmp    SHORT loc_14_013A
loc_14_0133:
        movzx  eax, byte ptr [esi + ebx + 0ah]
loc_14_0138:
        xor    edx, edx
loc_14_013A:
        mov    ecx, dword ptr [ebp + 4]
        movzx  esi, byte ptr [esi + ebx + 0bh]
        push   esi
        push   edx
        push   eax
        push   ecx
        push   ebx
        call   sub_14_0349
        add    esp, 14h
        inc    eax
        jne    SHORT loc_14_0160
        mov    edx, dword ptr [ebp]
        db  0BEh, 065h, 001h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
loc_14_0160:
        clc
        add    esp, 8
        ret
loc_14_0165:
        mov    ebx, edx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_14_017A
        mov    eax, 0ch
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_14_017A:
        ret
loc_14_017B:
        db  08Bh, 035h, 000h, 00Ah, 000h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    NEAR PTR loc_14_0232
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_14_01E5
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4d00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        mov    byte ptr [esi + ebx + 0bh], al
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0F6h, 005h, 01Bh, 00Ah, 000h, 000h, 001h
        je     SHORT loc_14_01E0
        call   0
loc_14_01E0:
        and    byte ptr [esi + ebx], 0fdh
loc_14_01E4:
        ret
loc_14_01E5:
        sub    esp, 6ch
        mov    edi, esp
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4d00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        or     ah, ah
        je     SHORT loc_14_0211
        mov    al, 0ffh
loc_14_0211:
        mov    byte ptr [esi + ebx + 0bh], al
        db  0CDh, 020h, 0BDh, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        jmp    SHORT loc_14_01E0
loc_14_0232:
        mov    byte ptr [esi + ebx + 0bh], 85h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_14_02BA
        cmp    word ptr [ebp + 1ch], 8
        db  00Fh, 084h, 0EBh, 0FFh, 0F1h, 0FFh
loc_14_024A:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 1ch], 5900h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, 4005h
        cmp    eax, 2
        je     SHORT loc_14_029D
        inc    esi
        cmp    eax, 3
        je     SHORT loc_14_029D
        inc    esi
        cmp    eax, 4
        je     SHORT loc_14_029D
        inc    esi
        cmp    eax, 5
        je     SHORT loc_14_029D
        inc    esi
        cmp    eax, 0bh
        je     SHORT loc_14_029D
        inc    esi
        cmp    eax, 20h
        je     SHORT loc_14_029D
        inc    esi
loc_14_029D:
        add    esi, 7c000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        pushad
        mov    esi, eax
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_14_02BA:
        movzx  eax, word ptr [ebp + 1ch]
        mov    byte ptr [esi + ebx + 0ah], al
        or     byte ptr [esi + ebx], 2
        jmp    NEAR PTR loc_14_01E4
        db  000h
sub_14_02CC:
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        xor    esi, esi
        xchg   dword ptr [edi + ebx + 8], esi
        test   esi, esi
        je     SHORT loc_14_02F9
        push   dword ptr [esi + 0ch]
        db  0CDh, 020h, 00Fh, 000h, 017h, 000h
        add    esp, 4
        mov    ecx, dword ptr [esi + 4]
        jecxz  loc_14_02F9
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_14_02F9:
        xor    esi, esi
        xchg   dword ptr [edi + ebx + 4], esi
        or     esi, esi
        je     SHORT loc_14_0309
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_14_0309:
        xor    esi, esi
        xchg   dword ptr [edi + ebx + 0ch], esi
        or     esi, esi
        je     SHORT loc_14_031F
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_14_031F:
        db  0E8h, 0EBh, 0FCh, 0F1h, 0FFh
        xchg   esi, eax
        xor    eax, eax
        xchg   word ptr [esi + 2ah], ax
        xor    edi, edi
        xchg   dword ptr [esi - 6], edi
        or     edi, edi
        je     SHORT loc_14_0337
        and    dword ptr [edi], 0
loc_14_0337:
        ret
loc_14_0338:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_14_0347
        db  083h, 025h, 00Ch, 00Ch, 000h, 000h, 000h
loc_14_0347:
        clc
        ret
sub_14_0349:
        push   ebp
        mov    ebp, esp
        test   byte ptr [ebp + 0ch], 4
        jne    NEAR PTR loc_14_03F3
        db  03Bh, 01Dh, 0CCh, 00Bh, 000h, 000h
        jne    SHORT loc_14_0363
        db  0E8h, 0CEh, 008h, 0F2h, 0FFh
loc_14_0363:
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        test   byte ptr [esi + ebx + 1ah], 1
        jne    SHORT loc_14_03DF
        pushad
        db  08Bh, 035h, 07Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_14_040C
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 18h]
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 0ch], edi
        xor    esi, esi
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [eax + 14h], esi
        cmp    ecx, 10001h
        jb     SHORT loc_14_03BF
        cmp    ecx, 10005h
        ja     SHORT loc_14_03BF
        test   edx, edx
        je     SHORT loc_14_03BF
        mov    esi, edx
        lea    edi, [eax + 10h]
        mov    edx, edi
        movsd
        movsd
loc_14_03BF:
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [esp + 14h], eax
        popad
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 1
        mov    ecx, 418h
        db  0BEh, 01Dh, 004h, 000h, 000h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_14_03F7
loc_14_03DF:
        mov    edx, ebx
        xor    eax, eax
        xor    ebx, ebx
        db  0BEh, 043h, 004h, 000h, 000h
        lea    ecx, [eax + 2]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_14_03F3:
        xor    eax, eax
        jmp    SHORT loc_14_041B
loc_14_03F7:
        xchg   edx, eax
        db  08Bh, 035h, 07Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT loc_14_040D
loc_14_040C:
        popad
loc_14_040D:
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        or     byte ptr [esi + ebx + 1ah], 1
        or     eax, 0ffffffffh
loc_14_041B:
        leave
        ret
loc_14_041D:
        jae    SHORT loc_14_042E
        mov    ebx, dword ptr [edx]
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        push   edx
        call   sub_14_04B0
        pop    edx
loc_14_042E:
        mov    eax, edx
        db  08Bh, 035h, 07Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        ret
loc_14_0443:
        mov    ebx, edx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_14_046A
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        or     byte ptr [esi + ebx + 1ah], 2
        cmp    byte ptr [esi + ebx + 1ah], 3
        jne    SHORT loc_14_046A
        mov    eax, 0ch
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_14_046A:
        ret
        db  000h
loc_14_046C:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     NEAR PTR loc_14_0529
        db  0E8h, 08Fh, 0FBh, 0F1h, 0FFh
        and    word ptr [eax + 2ah], 0
        db  08Bh, 035h, 074h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT sub_14_04B0
loc_14_0493:
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_14_04A8
        or     byte ptr [eax + 0bh], 80h
        pushad
        stc
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        jecxz  loc_14_04A7
        call   ecx
loc_14_04A7:
        popad
loc_14_04A8:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_14_0493
sub_14_04B0:
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_14_0529
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_14_0529
        push   ebx
        db  0CDh, 020h, 030h, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_14_04DA
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    eax, eax
        inc    eax
        db  0E8h, 038h, 0FDh, 0F1h, 0FFh
loc_14_04DA:
        pop    ebx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_14_0529
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        or     byte ptr [esi + ebx + 1ah], 1
        test   byte ptr [ebx], 10h
        jne    SHORT loc_14_0518
        test   byte ptr [ebx + 2], 1
        jne    SHORT loc_14_0529
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        or     eax, 2
        db  0CDh, 020h, 033h, 000h, 001h, 000h
        mov    eax, 1388h
        mov    ecx, 1
        db  0CDh, 020h, 0ECh, 080h, 001h, 000h
loc_14_0518:
        cmp    byte ptr [esi + ebx + 1ah], 3
        jne    SHORT loc_14_0529
        xor    eax, eax
        mov    al, 0ch
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_14_0529:
        ret
loc_14_052A:
        mov    ebx, dword ptr [ebp + 10h]
        and    byte ptr [ebp + 2ch], 0feh
        jmp    SHORT loc_14_054F
loc_14_0533:
        mov    edx, dword ptr [eax + 8]
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        test   edx, 100000h
        je     SHORT loc_14_054F
        db  0E8h, 015h, 006h, 0F2h, 0FFh
loc_14_054F:
        db  08Bh, 035h, 074h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_14_0570
loc_14_055D:
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_14_0568
        test   byte ptr [eax + 0bh], 80h
        jne    SHORT loc_14_0533
loc_14_0568:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_14_055D
loc_14_0570:
        ret
loc_14_0571:
        xor    ecx, ecx
        db  066h, 087h, 00Dh, 0E0h, 000h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_14_058A
        push   0
        push   ecx
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
loc_14_058A:
        ret
loc_14_058B:
        or     byte ptr [ebp + 2ch], 1
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_14_05AE
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_14_05B2
        xor    eax, eax
        mov    al, 13h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        jb     SHORT loc_14_05B2
loc_14_05AE:
        and    byte ptr [ebp + 2ch], 0feh
loc_14_05B2:
        ret
        db  000h
loc_14_05B4:
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   ebx, eax
        mov    esi, dword ptr [ebp]
        test   esi, esi
        je     NEAR PTR loc_14_066A
        test   byte ptr [esi + 0bh], 80h
        jne    NEAR PTR loc_14_066A
        mov    esi, dword ptr [esi + 4]
        mov    edi, ebx
        cld
        push   18h
        pop    ecx
        rep movsb
        mov    byte ptr [ebx + 18h], cl
        mov    ecx, dword ptr [ebx + 8]
        jecxz  loc_14_05F6
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 68h], eax
        mov    dword ptr [ebx + 6ch], edx
        mov    byte ptr [ebx + 70h], 0
loc_14_05F6:
        mov    eax, cs
        cmp    word ptr [ebx + 14h], ax
        jne    SHORT loc_14_0618
        or     byte ptr [ebx], 2
        lea    eax, [ebx + 18h]
        push   eax
        push   dword ptr [ebx + 10h]
        db  0CDh, 020h, 07Eh, 001h, 001h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_14_0618
        mov    byte ptr [ebx + 18h], al
loc_14_0618:
        mov    esi, ebx
        xor    ebx, ebx
        xchg   dword ptr [esi], ebx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_14_066A
        db  0E8h, 0E4h, 0F9h, 0F1h, 0FFh
        test   byte ptr [eax + 7], 10h
        je     SHORT loc_14_066A
        test   byte ptr [eax + 2], 8
        jne    SHORT loc_14_0668
        mov    ebx, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ebx + 44ch]
        shr    ecx, 1
        dec    ecx
        mov    eax, 0b0000h
        cmp    byte ptr [ebx + 449h], 7
        je     SHORT loc_14_0655
        or     ah, 80h
loc_14_0655:
        add    ax, word ptr [ebx + 44eh]
        lea    edi, [eax + ebx + 2]
        mov    eax, dword ptr [edi - 2]
        repe scasw
        jne    SHORT loc_14_066A
loc_14_0668:
        inc    dword ptr [esi]
loc_14_066A:
        ret
        db  000h
_SHELL_CancelAppyTimeEvent:
        push   esi
        mov    eax, dword ptr [esp + 8]
        db  08Bh, 035h, 0E0h, 00Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_14_0696
        cli
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        sti
        jb     SHORT loc_14_0696
        mov    edx, dword ptr [eax + 8]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, edx
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
loc_14_0696:
        pop    esi
        ret
_PTEXT ends
        end
