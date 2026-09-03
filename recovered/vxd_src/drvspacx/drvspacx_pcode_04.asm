; drvspacx_pcode_04.asm -- DRVSPACX object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        insd
        pop    edi
        jne    SHORT loc_4_0063
        popaw
        je     SHORT loc_4_0067
        xor    dword ptr [edx], esi
        bound  ebp, qword ptr [ecx + 74h]
        add    byte ptr [eax + eax], ah
        add    al, byte ptr [eax]
        sbb    dh, byte ptr [edx + 6dh]
        pop    edi
        jne    SHORT loc_4_0077
        imul   si, word ptr [edx + 73h], 5f74h
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
        jb     SHORT loc_4_0085
        jae    SHORT loc_4_008D
        arpl   word ptr [edi + ebp*2 + 72h], si
        add    byte ptr [esi], cl
        add    byte ptr [eax + eax], al
        db  00Fh, 072h, 06Dh, 05Fh, 075h, 05Fh, 068h, 06Fh, 073h, 074h, 05Fh, 069h, 072h, 070h, 074h, 000h
        db  04Ch, 000h, 001h, 000h, 01Bh, 072h, 06Dh, 05Fh, 075h, 05Fh, 068h, 06Fh, 073h, 074h, 05Fh, 073h
        db  065h, 063h, 074h, 06Fh, 072h, 05Fh, 073h, 069h, 07Ah, 065h, 05Fh, 070h, 06Fh, 077h, 032h, 000h
        db  00Ah, 000h
loc_4_0063:
        add    al, 0
        db  013h, 072h
loc_4_0067:
        insd
        pop    edi
        db  075h, 05Fh
        push   5f74736fh
        db  073h, 074h
        jb     SHORT loc_4_00D5
        je     SHORT loc_4_00DB
        db  067h
loc_4_0077:
        jns    SHORT loc_4_0079
loc_4_0079:
        pop    eax
        add    byte ptr [eax + eax], al
        adc    dword ptr [edx + 6dh], esi
        pop    edi
        db  075h, 05Fh
        db  068h, 06Fh
loc_4_0085:
        db  073h, 074h
        pop    edi
        db  076h, 06Fh
        insb
        pop    edi
        db  069h
loc_4_008D:
        add    byte ptr fs:[eax], bl
        add    byte ptr [edx], al
        add    byte ptr [edx], dl
        db  072h, 06Dh
        pop    edi
        jne    SHORT loc_4_00F8
        imul   ebp, dword ptr [edi + 5fh], 74726170h
        pop    edi
        jae    SHORT loc_4_010C
        db  07Ah, 065h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], dl
loc_4_00AB:
        db  072h, 06Dh
        pop    edi
        db  075h, 05Fh
        insb
        popad
        db  073h, 074h
        pop    edi
        arpl   word ptr [eax + 61h], bp
        imul   ebp, dword ptr [esi], 4005ch
        adc    dword ptr [edx + 6dh], esi
        pop    edi
        jne    SHORT loc_4_0124
        insb
        jo     SHORT loc_4_0127
        imul   esi, dword ptr fs:[ebx + 6bh], 70616d5fh
        add    byte ptr [eax], dl
        add    dword ptr [eax], eax
loc_4_00D5:
        adc    al, 72h
        insd
        pop    edi
        db  075h, 05Fh
loc_4_00DB:
        insd
        imul   esp, dword ptr fs:[ecx + 5fh], 63736564h
        jb     SHORT loc_4_0150
        db  070h, 074h
        add    byte ptr [edi], dl
        add    dword ptr [eax], eax
        adc    eax, 755f6d72h
        pop    edi
        db  072h, 065h
        insd
        outsd
loc_4_00F8:
        db  076h, 061h
        bound  ebp, qword ptr [ebp + 5fh]
        insd
        imul   esp, dword ptr fs:[ecx], 10053h
        adc    dword ptr [edx + 6dh], esi
        pop    edi
loc_4_010C:
        db  075h, 05Fh
        jae    SHORT loc_4_0175
        jno    SHORT loc_4_0187
loc_4_0112:
        outsb
        arpl   word ptr [ebp + 5fh], sp
        outsb
        outsd
        add    byte ptr [esi], dl
        add    dword ptr [eax], eax
        adc    byte ptr [edx + 6dh], dh
        pop    edi
        jne    SHORT loc_4_0183
loc_4_0124:
        db  073h, 06Ch
        outsd
loc_4_0127:
        je     SHORT loc_4_0188
        je     SHORT loc_4_018C
        imul   esp, dword ptr [ebp + 6eh], 0
        push   es
        add    byte ptr [eax + eax], al
        pop    ss
        db  072h
loc_4_0135:
        insd
        pop    edi
        jne    SHORT loc_4_0198
        jae    SHORT loc_4_01AC
        db  07Ah, 074h
        outsd
        jb     SHORT loc_4_01B3
        pop    edi
        jb     NEAR PTR loc_4_01A9
        pop    edi
        insb
        je     NEAR PTR loc_4_014B
loc_4_014B:
        push   ss
        add    byte ptr [edx], al
        add    byte ptr [ebx], bl
loc_4_0150:
        db  072h, 06Dh
        pop    edi
        db  075h, 05Fh
        db  073h, 071h
        jp     SHORT loc_4_01CD
        outsd
        jb     SHORT loc_4_01BB
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
        jb     SHORT loc_4_01C3
        outsd
        db  066h, 066h, 073h, 065h
        je     SHORT loc_4_016B
loc_4_016B:
        dec    esi
        add    byte ptr [ecx], al
        add    byte ptr [edi], cl
        jb     SHORT loc_4_01DF
        pop    edi
        db  075h, 05Fh
loc_4_0175:
        db  075h, 06Eh
        imul   esi, dword ptr [edi + ebx*2 + 63h], 65646fh
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_4_0183:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_4_0187:
        push   es
loc_4_0188:
        dec    ebp
        inc    esp
        inc    edx
        push   eax
loc_4_018C:
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], cl
        bound  ebp, qword ptr [edi + 6fh]
        je     SHORT loc_4_0202
loc_4_0198:
        insd
        jo     SHORT loc_4_019B
loc_4_019B:
        add    eax, dword ptr [eax]
        or     byte ptr [eax + 4], al
        outsd
        insd
        add    byte ptr [ebx], cl
        add    byte ptr [edx], al
        add    byte ptr [ebx], dl
loc_4_01A9:
        inc    edx
        push   eax
        inc    edx
loc_4_01AC:
        pop    edi
        inc    edx
        jns    SHORT loc_4_0224
        jae    NEAR PTR loc_4_0203
loc_4_01B3:
        db  065h, 072h, 053h
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
loc_4_01BB:
        add    byte ptr [16000100h], cl
        inc    edx
        push   eax
loc_4_01C3:
        inc    edx
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        db  073h, 050h
loc_4_01CD:
        db  065h, 072h, 043h
        insb
        db  075h, 073h
        je     SHORT loc_4_023A
        jb     SHORT loc_4_01D7
loc_4_01D7:
        push   cs
        add    byte ptr [edx], al
        add    byte ptr [edx + eax*2], dl
        push   eax
        inc    edx
loc_4_01DF:
        pop    edi
        push   edx
        db  065h, 073h, 065h
        db  072h, 076h
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_4_01F0
loc_4_01F0:
        adc    byte ptr [eax], al
        add    dword ptr [eax], eax
        adc    dword ptr [edx + 50h], eax
        inc    edx
        pop    edi
        dec    esi
        jne    SHORT loc_4_0269
        bound  esp, qword ptr [ebp + 72h]
        dec    edi
        inc    si
loc_4_0202:
        inc    ecx
loc_4_0203:
        push   esp
        jae    SHORT loc_4_0206
loc_4_0206:
        adc    dword ptr [eax], eax
        add    al, byte ptr [eax]
        adc    byte ptr [edx + 50h], al
loc_4_020D:
        inc    edx
        pop    edi
        push   edx
        outsd
        outsd
        je     SHORT loc_4_0259
        outsb
        je     SHORT loc_4_0289
        imul   esp, dword ptr [ebp + 73h], 2001300h
        add    byte ptr [ecx], dl
        inc    edx
        push   eax
        inc    edx
        pop    edi
loc_4_0224:
        push   esp
        outsd
        je     SHORT loc_4_0289
        insb
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_4_0231
loc_4_0231:
        adc    eax, 14000100h
        inc    edx
        push   eax
        inc    edx
        pop    edi
loc_4_023A:
        dec    ebp
loc_4_023B:
        imul   esp, dword ptr fs:[ecx + 44h], 72637365h
        imul   esi, dword ptr [eax + 74h], 1600726fh
        add    byte ptr [edx], al
        add    byte ptr [edx], dl
        inc    edx
        push   eax
        inc    edx
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
loc_4_0259:
        db  073h, 050h
        jb     NEAR PTR loc_4_02A4
        inc    ecx
        push   esp
        add    byte ptr [eax], bl
        add    byte ptr [edx], al
        add    byte ptr [edx + eax*2], dl
        push   eax
        inc    edx
loc_4_0269:
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_4_02C2
        db  065h, 072h, 054h
        db  072h, 061h
        arpl   word ptr [ebx], bp
        sbb    al, byte ptr [eax]
        add    al, byte ptr [eax]
        or     al, byte ptr [edx + 50h]
        inc    edx
        pop    edi
        dec    eax
        popad
        jae    NEAR PTR loc_4_0289
loc_4_0289:
        sbb    al, 0
        add    al, 0
        adc    al, byte ptr [edx + 50h]
        inc    edx
        pop    edi
        dec    eax
        imul   esp, dword ptr [esp + 65h], 6365536eh
        db  074h, 06Fh
        jb     SHORT loc_4_0312
        add    byte ptr [eax], ah
        add    byte ptr [eax + eax], al
loc_4_02A4:
        adc    al, 42h
        push   eax
        inc    edx
        pop    edi
        inc    edx
        imul   esp, dword ptr [edi + 54h], 6c61746fh
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_4_02B9
loc_4_02B9:
        das
        add    byte ptr [edx], al
        add    byte ptr [edx], cl
        pop    edi
        popaw
loc_4_02C2:
        je     SHORT loc_4_0323
        db  070h, 061h
        jb     SHORT loc_4_033C
        jae    SHORT loc_4_02CA
loc_4_02CA:
        sub    eax, 0e000200h
        imul   si, word ptr [edx + 73h], 5f74h
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
        jb     SHORT loc_4_02DD
loc_4_02DD:
        sub    eax, dword ptr [eax]
        add    al, byte ptr [eax]
        or     esp, dword ptr [esi + 69h]
        db  072h, 073h
        je     SHORT loc_4_0347
        popad
        db  074h, 061h
        add    byte ptr [ecx], ch
        add    byte ptr [edx], al
        add    byte ptr [ebx], cl
        imul   si, word ptr [edx + 73h], 5f74h
        db  072h, 06Fh
        outsd
        je     SHORT loc_4_02FD
loc_4_02FD:
        daa
        add    byte ptr [edx], al
        add    byte ptr [5f676f6ch], cl
        imul   ebp, dword ptr [esi + 74h], 616e7265h
        insb
        add    byte ptr [esi], bh
        add    byte ptr [edx], al
loc_4_0312:
        add    byte ptr [eax], cl
        insd
        popad
        js     SHORT loc_4_0377
        arpl   word ptr [ecx + 70h], sp
        add    byte ptr [ecx], bh
        add    byte ptr [eax + eax], al
        or     dword ptr [ebp + 61h], ebp
loc_4_0323:
        js     SHORT loc_4_0384
        imul   bp, word ptr [ebp], 35h
        add    al, 0
        or     dword ptr [ebp + 69h], ebp
        outsb
        pop    edi
        imul   bp, word ptr [ebp], 33h
        add    al, byte ptr [eax]
loc_4_033C:
        or     dword ptr [eax + 61h], esi
        jae    SHORT loc_4_03B4
        pop    edi
        outsd
        add    byte ptr [edx], dh
loc_4_0347:
        add    byte ptr [ecx], al
        add    byte ptr [ebx + esi*2], dl
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_4_03B2
        jo     SHORT loc_4_03B4
        db  075h, 06Eh
        imul   esi, dword ptr [edi + ebx*2 + 70h], 32776fh
        add    byte ptr es:[ecx], al
        add    byte ptr [ebx + esi*2], cl
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        pop    edi
        db  070h, 06Fh
        ja     SHORT loc_4_03A1
        add    byte ptr [eax + eax], ah
        add    al, byte ptr [eax]
        adc    dword ptr [ebx + 71h], esi
loc_4_0377:
        jp     SHORT loc_4_03ED
        outsd
        db  072h, 05Fh
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
        db  072h, 05Fh
loc_4_0384:
        outsd
        add    byte ptr [0b000100h], bh
        db  078h, 066h
        popad
        db  074h, 05Fh
        xor    dword ptr [edx], esi
        bound  ebp, qword ptr [ecx + 74h]
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [esp + eax*2], al
        push   eax
loc_4_03A1:
        inc    edx
        add    byte ptr [eax + eax], al
        add    dword ptr [eax], eax
        adc    dword ptr [eax + esi*2 + 62h], esp
        pop    edi
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
loc_4_03B2:
        jb     SHORT loc_4_0413
loc_4_03B4:
        insd
        popad
        db  073h, 06Bh
        add    byte ptr [12000100h], al
        db  064h, 070h, 062h
        pop    edi
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
        db  072h, 05Fh
        db  073h, 068h
        imul   esp, dword ptr [esi + 74h], 2001100h
        add    byte ptr [edi], cl
        db  064h, 070h, 062h
        pop    edi
        imul   esi, dword ptr fs:[edx + 5fh], 74636573h
        outsd
        jb     SHORT loc_4_03E4
loc_4_03E4:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     ah, byte ptr [eax + esi*2 + 62h]
        pop    edi
loc_4_03ED:
        db  064h, 072h, 069h
        jbe    SHORT loc_4_0457
        add    byte ptr [ebx], dl
        add    byte ptr [eax + eax], al
        adc    byte ptr [eax + esi*2 + 62h], ah
        pop    edi
        jb     NEAR PTR loc_4_0468
        jbe    SHORT loc_4_0466
        db  072h, 05Fh
        popad
        jb     NEAR PTR loc_4_0408
loc_4_0408:
        or     byte ptr [eax], al
        add    dword ptr [eax], eax
        push   cs
        db  064h, 070h, 062h
        pop    edi
        popaw
loc_4_0413:
        db  074h, 05Fh
        arpl   word ptr [edi + 75h], bp
        outsb
        je     SHORT loc_4_041B
loc_4_041B:
        sldt   word ptr [edx]
        add    byte ptr [5f627064h], cl
        popaw
        db  074h, 05Fh
        db  073h, 069h
        jp     SHORT loc_4_0491
        add    byte ptr [eax], bl
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        db  064h, 070h, 062h
        pop    edi
        insb
        popad
        jae    NEAR PTR loc_4_043C
loc_4_043C:
        push   es
        add    byte ptr [edx], al
        add    byte ptr [esi], cl
        db  064h, 070h, 062h
        pop    edi
        imul   si, word ptr [edx + 73h], 5f74h
        popaw
        je     SHORT loc_4_044F
loc_4_044F:
        or     eax, dword ptr [eax]
        add    al, byte ptr [eax]
        adc    dword ptr [eax + esi*2 + 62h], esp
loc_4_0457:
        pop    edi
        imul   si, word ptr [edx + 73h], 5f74h
        jae    SHORT loc_4_04C5
        arpl   word ptr [edi + ebp*2 + 72h], si
        add    byte ptr [edi], bl
loc_4_0466:
        add    byte ptr [edx], al
loc_4_0468:
        add    byte ptr [5f627064h], cl
        db  066h, 072h, 065h
        pop    edi
        arpl   word ptr [esi + 74h], bp
        add    byte ptr [10000200h], cl
        jo     NEAR PTR loc_4_04E1
        pop    edi
        insd
        popad
        db  078h, 05Fh
        arpl   word ptr [ebp + esi*2 + 73h], bp
        db  074h, 065h
        jb     SHORT loc_4_048C
loc_4_048C:
        pop    ss
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
loc_4_0491:
        jo     NEAR PTR loc_4_04F6
        pop    edi
        insd
        imul   esp, dword ptr fs:[ecx], 40019h
        or     eax, 5f627064h
        outsb
        js     NEAR PTR loc_4_051C
        pop    edi
        jo     NEAR PTR loc_4_050E
        add    byte ptr [0e000200h], bl
        jo     NEAR PTR loc_4_0517
        pop    edi
        outsb
        db  065h, 078h, 074h
        pop    edi
        jb     NEAR PTR loc_4_0523
        add    byte ptr gs:[ecx], cl
        add    byte ptr [edx], al
        add    byte ptr [ecx], dl
loc_4_04C5:
        db  064h, 070h, 062h
        pop    edi
        jb     SHORT loc_4_053A
        outsd
        je     SHORT loc_4_052D
        outsb
        db  074h, 072h
        imul   esp, dword ptr [ebp + 73h], 2000200h
        add    byte ptr [eax], dl
        jo     NEAR PTR loc_4_0540
        pop    edi
        jae    SHORT loc_4_0546
loc_4_04E1:
        arpl   word ptr [edi + ebp*2 + 72h], si
        pop    edi
        jae    SHORT loc_4_0551
        jp     SHORT loc_4_054F
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], cl
        jo     NEAR PTR loc_4_0555
        pop    edi
        jne    SHORT loc_4_0564
loc_4_04F6:
        imul   esi, dword ptr [eax + eax - 48h], 78h
        int    41h
        db  0BEh, 07Fh, 011h, 000h, 000h
        db  0E8h, 02Eh, 0EBh, 0FFh, 0FFh
        ret
        db  0B8h, 077h, 000h
loc_4_050E:
        add    byte ptr [eax], al
        mov    bl, 2
        int    41h
        or     ah, ah
        db  075h
loc_4_0517:
        sub    eax, 80ec81h
loc_4_051C:
        add    byte ptr [eax], al
        mov    esi, esp
        db  0B8h, 075h, 000h
loc_4_0523:
        add    byte ptr [eax], al
        int    41h
        mov    esi, dword ptr [esi + 14h]
        db  081h, 0C4h, 080h
loc_4_052D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 34ch], bh
        mov    eax, 79h
loc_4_053A:
        int    41h
        or     ax, ax
        db  075h
loc_4_0540:
        sub    eax, 1febde8bh
        db  0B8h
loc_4_0546:
        js     SHORT loc_4_0548
loc_4_0548:
        add    byte ptr [eax], al
        int    41h
        cmp    ebx, 1ah
loc_4_054F:
        jb     SHORT loc_4_056F
loc_4_0551:
        mov    esi, ebx
        db  0B9h, 04Ch
loc_4_0555:
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 79h], bh
        int    41h
        or     ax, ax
        jne    SHORT loc_4_056E
loc_4_0564:
        db  0BEh, 03Ah, 002h, 000h, 000h
        db  0E8h, 0CAh, 0EAh, 0FFh, 0FFh
loc_4_056E:
        ret
loc_4_056F:
        db  0B8h, 044h, 00Eh, 000h, 000h
loc_4_0574:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_056E
        cmp    byte ptr [eax + 88h], bl
        jne    SHORT loc_4_0574
        push   eax
        push   ebx
        mov    ebx, eax
        db  0BEh, 03Ah, 002h, 000h, 000h
        db  0E8h, 0A8h, 0EAh, 0FFh, 0FFh
        pop    ebx
        pop    eax
        jmp    SHORT loc_4_0574
        db  0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 075h, 03Ch, 08Ah, 025h, 067h
        db  00Eh, 000h, 000h, 0BAh, 0B4h, 015h, 000h, 000h, 0CDh, 020h, 009h, 000h, 08Bh, 004h, 0EBh, 054h
        db  09Ch, 0F7h, 046h, 00Ch, 000h, 000h, 000h, 0F0h, 075h, 049h, 08Bh, 0DEh, 0BEh, 0B1h, 00Ch, 000h
        db  000h, 0E8h, 06Eh, 0EAh, 0FFh, 0FFh, 09Dh, 09Ch, 074h, 039h, 068h, 0F0h, 00Bh, 000h, 000h, 0E8h
        db  03Ch, 0ECh, 0F9h, 0FFh, 083h, 0C4h, 004h, 0EBh, 02Ah, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h
        db  09Ch, 0BEh, 0B1h, 00Ch, 000h, 000h, 0E8h, 049h, 0EAh, 0FFh, 0FFh, 08Bh, 0F3h, 0CDh, 020h, 00Dh
        db  000h, 08Bh, 004h, 074h, 00Eh, 050h, 068h, 0FCh, 00Bh, 000h, 000h, 0E8h, 010h, 0ECh, 0F9h, 0FFh
        db  083h, 0C4h, 008h, 09Dh, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h
        db  075h, 04Fh, 08Ah, 025h, 068h, 00Eh, 000h, 000h, 0BAh, 029h, 016h, 000h, 000h, 0CDh, 020h, 009h
        db  000h, 08Bh, 004h, 0EBh, 067h, 09Ch, 08Bh, 05Eh, 00Ch, 081h, 0E3h, 000h, 000h, 000h, 0F0h, 081h
        db  0FBh, 000h, 000h, 000h, 010h, 075h, 054h, 08Bh, 0DEh, 0BEh, 04Eh, 00Dh, 000h, 000h, 0F6h, 043h
        db  00Ch, 0FFh, 074h, 005h, 0BEh, 005h, 00Eh, 000h, 000h, 0E8h, 0E6h, 0E9h, 0FFh, 0FFh, 09Dh, 09Ch
        db  074h, 039h, 068h, 010h, 00Ch, 000h, 000h, 0E8h, 0B4h, 0EBh, 0F9h, 0FFh, 083h, 0C4h, 004h, 0EBh
        db  02Ah, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 09Ch, 0BEh, 04Eh, 00Dh, 000h, 000h, 0E8h, 0C1h
        db  0E9h, 0FFh, 0FFh, 08Bh, 0F3h, 0CDh, 020h, 00Dh, 000h, 08Bh, 004h, 074h, 00Eh, 050h, 068h, 01Ch
        db  00Ch, 000h, 000h, 0E8h, 088h, 0EBh, 0F9h, 0FFh, 083h, 0C4h, 008h, 09Dh, 0C3h, 068h, 030h, 00Ch
        db  000h, 000h, 0E8h, 079h, 0EBh, 0F9h, 0FFh, 083h, 0C4h, 004h, 0C3h, 068h, 060h, 00Ch, 000h, 000h
        db  0E8h, 06Bh, 0EBh, 0F9h, 0FFh, 083h, 0C4h, 004h, 0C3h, 0C3h, 0B8h, 078h, 000h, 000h, 000h, 0CDh
        db  041h, 083h, 0FBh, 01Ah, 072h, 00Bh, 0BEh, 04Bh, 00Eh, 000h, 000h, 0E8h, 074h, 0E9h, 0FFh, 0FFh
        db  0C3h, 0B8h, 044h, 00Eh, 000h, 000h, 08Bh, 000h, 00Bh, 0C0h, 074h, 0F4h, 038h, 098h, 088h, 000h
        db  000h, 000h, 075h, 0F2h, 053h, 08Bh, 098h, 0B8h, 000h, 000h, 000h, 00Bh, 0DBh, 074h, 00Ch, 0BEh
        db  04Bh, 00Eh, 000h, 000h, 050h, 0E8h, 04Ah, 0E9h, 0FFh, 0FFh, 058h, 05Bh, 0EBh, 0D8h, 068h, 090h
        db  00Ch, 000h, 000h, 0E8h, 018h, 0EBh, 0F9h, 0FFh, 083h, 0C4h, 004h, 0C3h, 0B8h, 077h, 000h, 000h
        db  000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 075h, 02Dh, 08Bh, 01Dh, 040h, 00Eh, 000h, 000h, 066h
        db  083h, 0FBh, 0FFh, 074h, 020h, 0BEh, 096h, 013h, 000h, 000h, 0E8h, 015h, 0E9h, 0FFh, 0FFh, 08Bh
        db  05Bh, 019h, 066h, 083h, 0FBh, 0FFh, 074h, 00Dh, 00Fh, 0B7h, 0C3h, 0C1h, 0EBh, 010h, 0C1h, 0E3h
        db  004h, 003h, 0D8h, 0EBh, 0E0h, 0C3h, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 083h, 0FBh, 019h
        db  076h, 00Bh, 0BEh, 096h, 013h, 000h, 000h, 0E8h, 0E8h, 0E8h, 0FFh, 0FFh, 0C3h, 00Fh, 0B6h, 0C3h
        db  0E8h, 002h, 0EAh, 0F9h, 0FFh, 072h, 00Ch, 0BEh, 096h, 013h, 000h, 000h, 08Bh, 0DFh, 0E8h, 0D1h
        db  0E8h, 0FFh, 0FFh, 0C3h, 068h, 0C0h, 00Ch, 000h, 000h, 0E8h, 0A2h, 0EAh, 0F9h, 0FFh, 083h, 0C4h
        db  004h, 0C3h, 000h, 000h, 000h, 000h, 04Eh, 04Fh, 054h, 020h, 000h, 044h, 03Fh, 03Fh, 053h, 050h
        db  041h, 043h, 03Fh, 000h, 0F0h, 017h, 000h, 000h, 00Ah, 000h, 000h, 000h, 029h, 018h, 000h, 000h
        db  091h, 016h, 000h, 000h, 033h, 018h, 000h, 000h, 09Fh, 016h, 000h, 000h, 044h, 018h, 000h, 000h
        db  0ADh, 016h, 000h, 000h, 00Dh, 018h, 000h, 000h, 0F9h, 014h, 000h, 000h, 019h, 018h, 000h, 000h
        db  00Bh, 015h, 000h, 000h, 056h, 018h, 000h, 000h, 0AEh, 016h, 000h, 000h, 070h, 018h, 000h, 000h
        db  094h, 015h, 000h, 000h, 07Fh, 018h, 000h, 000h, 009h, 016h, 000h, 000h, 08Fh, 018h, 000h, 000h
        db  02Ah, 000h, 000h, 000h, 0ACh, 018h, 000h, 000h, 0F2h, 016h, 000h, 000h, 0C5h, 018h, 000h, 000h
        db  068h, 017h, 000h, 000h, 0E0h, 018h, 000h, 000h, 000h, 017h, 000h, 000h, 054h, 06Fh, 067h, 067h
        db  06Ch, 065h, 020h, 065h, 06Dh, 070h, 074h, 079h, 020h, 066h, 069h, 065h, 06Ch, 064h, 020h, 070h
        db  072h, 069h, 06Eh, 074h, 069h, 06Eh, 067h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 04Dh, 044h
        db  042h, 050h, 042h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 044h, 049h, 053h, 04Bh, 05Fh, 055h
        db  04Eh, 049h, 054h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 06Ch, 06Fh, 067h, 02Eh, 000h, 044h
        db  075h, 06Dh, 070h, 020h, 073h, 074h, 061h, 074h, 069h, 073h, 074h, 069h, 063h, 073h, 02Eh, 000h
        db  043h, 06Ch, 065h, 061h, 072h, 020h, 073h, 074h, 061h, 074h, 069h, 073h, 074h, 069h, 063h, 073h
        db  02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 072h, 065h, 061h, 06Ch, 02Dh, 06Dh, 06Fh, 064h, 065h
        db  020h, 044h, 049h, 053h, 04Bh, 05Fh, 055h, 04Eh, 049h, 054h, 02Eh, 000h, 044h, 075h, 06Dh, 070h
        db  020h, 043h, 041h, 043h, 048h, 045h, 048h, 044h, 052h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h
        db  043h, 041h, 043h, 048h, 045h, 043h, 04Ch, 055h, 053h, 02Eh, 000h, 054h, 06Fh, 067h, 067h, 06Ch
        db  065h, 020h, 063h, 06Ch, 075h, 073h, 074h, 065h, 072h, 020h, 063h, 068h, 065h, 063h, 06Bh, 073h
        db  075h, 06Dh, 06Dh, 069h, 06Eh, 067h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 070h, 061h, 074h
        db  068h, 020h, 063h, 06Fh, 076h, 065h, 072h, 061h, 067h, 065h, 020h, 064h, 061h, 074h, 061h, 02Eh
        db  000h, 050h, 075h, 074h, 020h, 072h, 065h, 066h, 065h, 072h, 065h, 06Eh, 063h, 065h, 020h, 06Dh
        db  061h, 072h, 06Bh, 020h, 069h, 06Eh, 020h, 06Ch, 06Fh, 067h, 02Eh, 000h, 044h, 075h, 06Dh, 070h
        db  020h, 044h, 04Fh, 053h, 020h, 044h, 050h, 042h, 02Eh, 000h, 05Bh, 058h, 05Dh, 020h, 02Dh, 020h
        db  045h, 078h, 069h, 074h, 020h, 06Dh, 065h, 06Eh, 075h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h
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
loc_4_0AB7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
loc_4_0BF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_4_0E69:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0F85:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 76h], ah
        db  0F0h, 08Ah, 0D3h, 0E8h, 076h, 018h, 0FDh, 0FFh, 00Bh, 0C0h, 00Fh, 085h, 017h, 001h, 000h, 000h
        db  052h, 06Ah, 000h, 068h, 000h, 002h, 000h, 000h, 0CDh, 020h, 0A9h, 000h, 001h, 000h, 083h, 0C4h
        db  008h, 05Ah, 00Bh, 0C0h, 00Fh, 084h, 0FDh, 000h, 000h, 000h, 08Bh, 0F8h, 08Ah, 056h, 053h, 0BEh
        db  076h, 007h, 000h, 000h, 08Bh, 0DFh, 0FCh, 0ACh, 0AAh, 00Ah, 0C0h, 075h, 0FAh, 08Ah, 0C6h, 004h
        db  041h, 088h, 003h, 00Fh, 0B6h, 0C2h, 0B2h, 00Ah, 0F6h, 0F2h, 080h, 0C4h, 030h, 088h, 067h, 0FEh
        db  032h, 0E4h, 0F6h, 0F2h, 080h, 0C4h, 030h, 088h, 067h, 0FDh, 004h, 030h, 088h, 047h, 0FCh, 066h
        db  0C7h, 043h, 004h, 052h, 056h, 066h
loc_4_1068:
        mov    dword ptr [ebp + 1ch], 0cbc13d20h
        add    al, 66h
loc_4_1071:
        mov    dword ptr [ebp + 3ch], ebx
        rol    ebx, 4
        mov    eax, ebx
        and    ax, 0fh
        mov    word ptr [ebp + 14h], ax
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        je     SHORT loc_4_10C3
        mov    word ptr [ebx + 4], 4c42h
        mov    word ptr [ebp + 1ch], 3d20h
        ror    ebx, 4
        mov    word ptr [ebp + 3ch], bx
        rol    ebx, 4
        mov    eax, ebx
        and    ax, 0fh
        mov    word ptr [ebp + 14h], ax
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_4_111E
loc_4_10C3:
        mov    ax, word ptr [ebp + 1ch]
        mov    word ptr [ebp + 10h], ax
        ror    ebx, 4
        mov    word ptr [ebp + 3ch], bx
        rol    ebx, 4
        mov    eax, ebx
        and    ax, 0fh
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 18h], 200h
        mov    byte ptr [ebp + 1dh], 3fh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ebp + 2ch], 1
        jne    SHORT loc_4_110D
        cmp    word ptr [ebp + 1ch], 200h
        jne    SHORT loc_4_110D
        mov    esi, ebx
        db  0E8h, 0E8h, 0FFh, 0FCh, 0FFh
        mov    dword ptr [esp + 4], ecx
loc_4_110D:
        pushfd
        mov    byte ptr [ebp + 1dh], 3eh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        popfd
loc_4_111E:
        clc
        pushfd
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        popfd
        je     SHORT loc_4_1131
        stc
        db  0C6h, 005h, 02Ch, 008h, 000h, 000h, 001h
loc_4_1131:
        popad
        ret
        db  060h, 06Ah, 000h, 06Ah, 020h, 0CDh, 020h, 0A9h, 000h, 001h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h
        db  074h, 051h, 08Bh, 0F8h, 08Bh, 0D8h, 08Bh, 035h, 048h, 00Eh, 000h, 000h, 0FCh, 0ACh, 0AAh, 00Ah
        db  0C0h, 075h, 0FAh, 08Bh, 044h, 024h, 01Ch, 000h, 003h, 066h, 0C7h, 045h, 01Ch, 000h, 043h, 0C1h
        db  0CBh, 004h, 066h, 089h, 05Dh, 03Ch, 0C1h, 0C3h, 004h, 066h, 083h, 0E3h, 00Fh, 066h, 089h, 05Dh
        db  014h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0F6h, 045h, 02Ch, 001h
        db  075h, 00Bh, 08Bh, 044h, 024h, 01Ch, 00Fh, 0ABh, 005h, 0C0h, 007h, 000h, 000h, 0CDh, 020h, 0AAh
        db  000h, 001h, 000h, 061h, 0C3h
sub_4_1198:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 52h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ebx, word ptr [ebp + 38h]
        shl    ebx, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    ebx, ecx
        movzx  eax, word ptr [ebx]
        movzx  ebx, word ptr [ebx + 2]
        shl    ebx, 4
        add    ebx, eax
        db  089h, 01Dh, 040h, 00Eh, 000h, 000h
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    NEAR PTR loc_4_1358
        cmp    word ptr [ebp + 10h], 444dh
        jne    NEAR PTR loc_4_1358
        db  0C6h, 005h, 066h, 00Eh, 000h, 000h, 001h
        movzx  ecx, word ptr [ebp + 14h]
        db  066h, 089h, 00Dh, 076h, 00Dh, 000h, 000h
        db  066h, 039h, 00Dh, 074h, 00Dh, 000h, 000h
        jb     SHORT loc_4_1235
        db  066h, 089h, 00Dh, 074h, 00Dh, 000h, 000h
loc_4_1235:
        mov    bl, byte ptr [ebp + 19h]
        db  088h, 01Dh, 078h, 00Dh, 000h, 000h
        movzx  ecx, byte ptr [ebp + 18h]
        sub    cl, 41h
        db  088h, 00Dh, 07Ah, 00Dh, 000h, 000h
        db  066h, 083h, 03Dh, 074h, 00Dh, 000h, 000h, 00Ah
        jb     SHORT loc_4_1283
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 0bh
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_4_1283
        movzx  eax, word ptr [ebp + 18h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 14h]
        db  0A3h, 064h, 00Dh, 000h, 000h
loc_4_1283:
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 8
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_4_12B5
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 072h, 00Dh, 000h, 000h
        mov    ax, word ptr [ebp + 18h]
        db  066h, 0A3h, 070h, 00Dh, 000h, 000h
loc_4_12B5:
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 9
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    NEAR PTR loc_4_1358
        mov    al, byte ptr [ebp + 18h]
        db  0A2h, 079h, 00Dh, 000h, 000h
        mov    ax, word ptr [ebp + 14h]
        shl    eax, 10h
        mov    ax, word ptr [ebp]
        db  0A3h, 06Ch, 00Dh, 000h, 000h
loc_4_12EF:
        db  0C6h, 005h, 02Ch, 008h, 000h, 000h, 001h
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000000h
        ja     SHORT loc_4_1358
        db  0C6h, 005h, 02Ch, 008h, 000h, 000h, 000h
        db  0E8h, 0BEh, 013h, 0FDh, 0FFh
        db  066h, 081h, 03Dh, 076h, 00Dh, 000h, 000h, 001h, 001h
        je     SHORT loc_4_1358
        db  066h, 083h, 03Dh, 076h, 00Dh, 000h, 000h, 00Dh
        je     SHORT loc_4_1358
        db  080h, 03Dh, 07Bh, 00Dh, 000h, 000h, 000h
        jne    SHORT loc_4_1358
        db  00Fh, 0B7h, 03Dh, 01Ah, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Ch, 000h, 000h, 000h
loc_4_1341:
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 80001000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
loc_4_1358:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
        db  000h, 033h, 0C0h, 0BFh, 026h, 008h, 000h, 000h
loc_4_1377:
        mov    byte ptr [edi + 7], 33h
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        or     ecx, ecx
        jne    NEAR PTR loc_4_1531
        db  00Fh, 0B7h, 005h, 01Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 048h, 00Eh, 000h, 000h
        db  00Fh, 0B7h, 035h, 020h, 000h, 000h, 000h
        db  003h, 035h, 004h, 000h, 000h, 000h
        db  0BFh, 06Ah, 031h, 000h, 000h
        mov    ecx, 0bh
        cld
        rep movsb
        db  0B8h, 000h, 000h, 000h, 000h
        and    eax, 0fffff000h
        db  0BAh, 04Ch, 04Ch, 000h, 000h
        add    edx, 0fffh
        and    edx, 0fffff000h
        sub    edx, eax
        shr    eax, 0ch
        shr    edx, 0ch
        db  0A3h, 024h, 00Eh, 000h, 000h
        db  089h, 015h, 018h, 00Eh, 000h, 000h
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        db  0A3h, 038h, 00Eh, 000h, 000h
        call   sub_4_16CF
        db  08Bh, 015h, 034h, 00Eh, 000h, 000h
        sub    edx, 64h
        sbb    eax, eax
        and    edx, eax
        add    edx, 64h
        db  089h, 015h, 034h, 00Eh, 000h, 000h
        call   sub_4_1632
        db  08Bh, 015h, 030h, 00Eh, 000h, 000h
        sub    edx, 4
        sbb    eax, eax
        and    edx, eax
        add    edx, 4
        xor    eax, eax
        db  0BFh, 012h, 008h, 000h, 000h
        call   sub_4_15D0
        cmp    eax, 1
        sbb    eax, eax
        and    edx, eax
        db  089h, 015h, 02Ch, 00Eh, 000h, 000h
        db  00Fh, 0B6h, 005h, 065h, 00Eh, 000h, 000h
        db  0BFh, 0E8h, 007h, 000h, 000h
        call   sub_4_15D0
        db  0A2h, 065h, 00Eh, 000h, 000h
        db  00Fh, 0B6h, 005h, 064h, 00Eh, 000h, 000h
        db  0BFh, 0DEh, 007h, 000h, 000h
        call   sub_4_15D0
        db  0A2h, 064h, 00Eh, 000h, 000h
        call   sub_4_1198
        call   sub_4_1541
        db  00Fh, 0B6h, 015h, 066h, 00Eh, 000h, 000h
        db  00Bh, 015h, 064h, 00Dh, 000h, 000h
        je     NEAR PTR loc_4_1531
        db  0CDh, 020h, 000h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_4_1531
        cmp    eax, 100h
        jne    NEAR PTR loc_4_1531
        mov    ecx, 10h
        db  0BEh, 009h, 01Ch, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 08Bh, 004h
        jb     NEAR PTR loc_4_1531
        db  0A2h, 068h, 00Eh, 000h, 000h
        mov    ecx, 2
        db  0BEh, 03Ch, 02Eh, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 08Bh, 004h
        jb     SHORT loc_4_1533
        db  0A2h, 067h, 00Eh, 000h, 000h
        call   sub_4_175C
        jb     SHORT loc_4_1531
        db  083h, 03Dh, 03Ch, 00Eh, 000h, 000h, 000h
        jne    SHORT loc_4_14F8
        call   sub_4_1685
        jb     SHORT loc_4_14E7
        call   sub_4_1628
        jae    SHORT loc_4_14F8
loc_4_14E7:
        db  083h, 025h, 02Ch, 00Eh, 000h, 000h, 000h
        db  0C7h, 005h, 03Ch, 00Eh, 000h, 000h, 04Ch, 00Eh, 000h, 000h
loc_4_14F8:
        db  0C7h, 005h, 0A8h, 007h, 000h, 000h, 030h, 000h, 000h, 000h
        db  0C7h, 005h, 0ACh, 007h, 000h, 000h, 058h, 009h, 000h, 000h
        db  068h, 09Ch, 007h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        db  066h, 083h, 03Dh, 0C8h, 007h, 000h, 000h, 001h
        je     SHORT loc_4_152E
        db  066h, 083h, 03Dh, 0C8h, 007h, 000h, 000h, 002h
        jne    SHORT loc_4_1531
loc_4_152E:
        xor    eax, eax
        ret
loc_4_1531:
        stc
        ret
loc_4_1533:
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 08Bh, 004h
        jmp    SHORT loc_4_1531
sub_4_1541:
        db  0BFh, 0D4h, 007h, 000h, 000h
        xor    edx, edx
        push   edx
        call   sub_4_160A
        mov    esi, edx
        pop    edx
        jb     SHORT loc_4_1558
        call   sub_4_156F
loc_4_1558:
        db  0BEh, 094h, 008h, 000h, 000h
        call   sub_4_156F
        db  00Bh, 015h, 064h, 00Dh, 000h, 000h
        db  089h, 015h, 064h, 00Dh, 000h, 000h
        ret
sub_4_156F:
        xor    eax, eax
        cld
loc_4_1572:
        lodsb
        or     al, al
        je     SHORT loc_4_15CF
        cmp    al, 20h
        je     SHORT loc_4_1572
        cmp    al, 9
        je     SHORT loc_4_1572
        dec    esi
        cmp    word ptr [esi], 30h
        je     SHORT loc_4_15CF
        cmp    word ptr [esi], 31h
        jne    SHORT loc_4_1591
        or     edx, 0ffffffffh
        jmp    SHORT loc_4_15CF
loc_4_1591:
        lodsb
        or     al, al
        je     SHORT loc_4_15CF
        sub    al, 61h
        jae    SHORT loc_4_159E
        add    al, 20h
        jae    SHORT loc_4_1591
loc_4_159E:
        cmp    al, 19h
        ja     SHORT loc_4_1591
        cmp    byte ptr [esi], 2dh
        jne    SHORT loc_4_15CA
        mov    cl, byte ptr [esi + 1]
        sub    cl, 61h
        jae    SHORT loc_4_15B4
        add    cl, 20h
        jae    SHORT loc_4_15CA
loc_4_15B4:
        cmp    cl, 19h
        ja     SHORT loc_4_15CA
        cmp    al, cl
        ja     SHORT loc_4_1591
loc_4_15BD:
        bts    edx, eax
        inc    eax
        cmp    al, cl
        jbe    SHORT loc_4_15BD
        add    esi, 2
        jmp    SHORT loc_4_1591
loc_4_15CA:
        bts    edx, eax
        jmp    SHORT loc_4_1591
loc_4_15CF:
        ret
sub_4_15D0:
        push   eax
        push   ecx
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000000h
        pop    ecx
        pop    eax
        ja     SHORT loc_4_15EC
        db  0BEh, 01Dh, 008h, 000h, 000h
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
loc_4_15EC:
        ret
        db  050h, 051h, 0CDh, 020h, 011h, 001h, 001h, 000h, 03Dh, 000h, 000h, 000h, 020h, 059h, 058h, 077h
        db  00Bh, 0BEh, 01Dh, 008h, 000h, 000h, 0CDh, 020h, 0ABh, 000h, 001h, 000h, 0C3h
sub_4_160A:
        push   eax
        push   ecx
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000001h
        pop    ecx
        pop    eax
        cmc
        jb     SHORT loc_4_1627
        db  0BEh, 01Dh, 008h, 000h, 000h
        db  0CDh, 020h, 0B3h, 000h, 001h, 000h
loc_4_1627:
        ret
sub_4_1628:
        db  0CDh, 020h, 000h, 000h, 042h, 000h
        jae    SHORT loc_4_163A
        jmp    SHORT loc_4_164F
sub_4_1632:
        db  0CDh, 020h, 000h, 000h, 042h, 000h
        jb     SHORT loc_4_164F
loc_4_163A:
        cmp    eax, 40ah
        jb     SHORT loc_4_164F
        sub    eax, eax
        db  0CDh, 020h, 002h, 000h, 042h, 000h
        db  0FEh, 005h, 05Eh, 00Eh, 000h, 000h
loc_4_164F:
        mov    eax, 4a12h
        mov    edx, 4d524349h
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        cmp    edx, 4943524dh
        jne    SHORT loc_4_167C
        movzx  eax, word ptr [edi + 0ch]
        test   eax, 100h
        je     SHORT loc_4_167C
        db  089h, 03Dh, 03Ch, 00Eh, 000h, 000h
        jmp    SHORT loc_4_1684
loc_4_167C:
        db  0C6h, 005h, 05Eh, 00Eh, 000h, 000h, 000h
        stc
loc_4_1684:
        ret
sub_4_1685:
        pushad
        db  0BAh, 0F9h, 007h, 000h, 000h
        mov    eax, 1
        call   sub_4_16A8
        jae    SHORT loc_4_16A6
        db  0BAh, 002h, 008h, 000h, 000h
        mov    eax, 1
        call   sub_4_16A8
loc_4_16A6:
        popad
        ret
sub_4_16A8:
        sub    esp, 50h
        mov    edi, esp
        push   eax
        push   edx
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    esi, edx
        cld
        rep movsb
        pop    esi
loc_4_16BB:
        lodsb
        stosb
        test   al, al
        jne    SHORT loc_4_16BB
        pop    eax
        mov    edx, esp
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        lea    esp, [esp + 50h]
        ret
sub_4_16CF:
        sub    esp, 0ch
        db  08Dh, 035h, 02Eh, 008h, 000h, 000h
        lea    edi, [esp + 8]
        push   edi
        push   esi
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        cmc
        jb     SHORT loc_4_1754
        db  0BBh, 0D0h, 008h, 000h, 000h
loc_4_16F7:
        add    ebx, 14h
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        je     SHORT loc_4_1745
        mov    eax, dword ptr [ebx + 8]
        lea    edx, [esp + 4]
        mov    esi, dword ptr [ebx + 0ch]
        lea    edi, [esp]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp + 8]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_173B
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [ebx + 4], eax
        je     SHORT loc_4_16F7
        cmp    eax, 4
        jne    SHORT loc_4_173B
        cmp    dword ptr [ebx + 4], 3
        je     SHORT loc_4_16F7
loc_4_173B:
        mov    eax, dword ptr [ebx + 10h]
        mov    esi, dword ptr [ebx + 0ch]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_4_16F7
loc_4_1745:
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        clc
loc_4_1754:
        lea    esp, [esp + 0ch]
        ret
        db  000h, 000h, 000h
sub_4_175C:
        db  0E8h, 008h, 007h, 0FDh, 0FFh
        jb     SHORT loc_4_1798
        db  0A3h, 028h, 048h, 000h, 000h
        mov    ecx, 8
        db  0BBh, 028h, 048h, 000h, 000h
        mov    edi, 20000008h
loc_4_1777:
        dec    edi
        db  08Ah, 025h, 068h, 00Eh, 000h, 000h
        mov    al, 33h
        push   ecx
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    ecx
        jae    SHORT loc_4_1798
        or     esi, esi
        je     SHORT loc_4_1798
        db  089h, 034h, 08Dh, 004h, 048h, 000h, 000h
        loop   loc_4_1777
        clc
loc_4_1798:
        ret
        db  000h, 000h, 000h, 058h, 058h, 058h, 058h, 058h, 058h, 058h, 058h, 040h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 052h, 056h, 053h, 050h, 041h, 043h, 045h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 041h, 075h, 074h, 06Fh, 04Dh
        db  06Fh, 075h, 06Eh, 074h, 000h, 046h, 041h, 054h, 033h, 032h, 048h, 06Fh, 073h, 074h, 000h, 049h
        db  06Eh, 076h, 061h, 06Ch, 069h, 064h, 061h, 074h, 065h, 042h, 069h, 074h, 046h, 041h, 054h, 000h
        db  04Dh, 052h, 043h, 049h, 02Eh, 056h, 058h, 044h, 000h, 056h, 04Dh, 04Dh, 033h, 032h, 05Ch, 04Dh
        db  052h, 043h, 049h, 032h, 02Eh, 056h, 058h, 044h, 000h, 04Eh, 06Fh, 043h, 06Fh, 06Dh, 070h, 072h
        db  065h, 073h, 073h, 000h, 044h, 052h, 056h, 053h, 050h, 041h, 043h, 045h, 000h, 044h, 052h, 056h
        db  053h, 050h, 041h, 043h, 058h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h
        db  065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 05Ch, 046h, 069h, 06Ch, 065h, 053h, 079h, 073h, 074h, 065h, 06Dh
        db  000h, 041h, 075h, 074h, 06Fh, 04Dh, 06Fh, 075h, 06Eh, 074h, 044h, 072h, 069h, 076h, 065h, 073h
        db  000h, 043h, 06Fh, 06Dh, 070h, 072h, 065h, 073h, 073h, 069h, 06Fh, 06Eh, 041h, 06Ch, 067h, 06Fh
        db  072h, 069h, 074h, 068h, 06Dh, 000h, 043h
loc_4_1880:
        outsd
        insd
        jo     SHORT loc_4_18F6
        jae    NEAR PTR loc_4_18FA
        imul   ebp, dword ptr [edi + 6eh], 65726854h
        jae    SHORT loc_4_18F8
        outsd
        insb
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pop    edx
        or     byte ptr [eax], al
        db  000h, 094h, 008h, 000h, 000h, 000h, 000h
loc_4_18F6:
        add    byte ptr [eax], al
loc_4_18F8:
        add    al, 0
loc_4_18FA:
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        push   8
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        jg     SHORT loc_4_191E
        add    byte ptr [eax], al
        xor    al, 0eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_191E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_4_1B80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PTEXT ends
        end
