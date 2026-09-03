; drvspacx_icode_03.asm -- DRVSPACX object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        xor    dl, dl
        mov    eax, 1
        int    41h
        ret
        db  0B8h, 07Ah, 017h, 000h, 000h, 080h, 035h, 079h, 017h, 000h, 000h, 001h, 075h, 003h, 083h, 0C0h
        db  004h, 050h, 068h, 034h, 00Bh, 000h, 000h, 0E8h, 0EEh, 001h, 0FAh, 0FFh, 083h, 0C4h, 008h, 0C3h
        db  068h, 058h, 00Bh, 000h, 000h, 0E8h, 0E0h, 001h, 0FAh, 0FFh, 083h, 0C4h, 004h, 0C3h, 083h, 0ECh
        db  00Ch, 0FCh, 0ADh, 089h, 004h, 024h, 0B8h, 04Fh, 000h, 000h, 000h, 033h, 0D2h, 0F7h, 034h, 024h
        db  089h, 044h, 024h, 004h, 0B8h, 04Fh, 000h, 000h, 000h, 02Bh, 0C2h, 02Bh, 044h, 024h, 004h, 089h
        db  044h, 024h, 008h, 0ADh, 08Bh, 0C8h, 0ACh, 053h, 056h, 068h, 088h, 00Bh, 000h, 000h, 0E8h, 0A7h
        db  001h, 0FAh, 0FFh, 083h, 0C4h, 00Ch, 00Fh, 0B6h, 0C0h, 003h, 0F0h, 0C7h, 005h, 0E4h, 00Ah, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_3_007F:
        lodsd
        movzx  edi, ax
        shr    eax, 10h
        mov    edx, eax
        lodsb
        cmp    dx, -1
        je     SHORT loc_3_00BD
        cmp    edx, 2
        jne    SHORT loc_3_00C2
        movzx  edx, word ptr [edi + ebx]
        db  080h, 03Dh, 079h, 017h, 000h, 000h, 000h
        je     SHORT loc_3_00A9
        or     edx, edx
        je     NEAR PTR loc_3_0182
loc_3_00A9:
        push   edx
        push   esi
        db  068h, 0A0h, 00Bh, 000h, 000h
        db  0E8h, 05Fh, 001h, 0FAh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_3_01D9
loc_3_00BD:
        lea    edx, [edi + ebx]
        jmp    SHORT loc_3_00DB
loc_3_00C2:
        cmp    edx, 4
        jne    SHORT loc_3_00EF
        mov    edx, dword ptr [edi + ebx]
        db  080h, 03Dh, 079h, 017h, 000h, 000h, 000h
        je     SHORT loc_3_00DB
        or     edx, edx
        je     NEAR PTR loc_3_0182
loc_3_00DB:
        push   edx
        push   esi
        db  068h, 0ACh, 00Bh, 000h, 000h
        db  0E8h, 02Dh, 001h, 0FAh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_3_01D9
loc_3_00EF:
        test   edx, 0c000h
        jne    SHORT loc_3_0139
        db  080h, 03Dh, 079h, 017h, 000h, 000h, 000h
        je     SHORT loc_3_010B
        cmp    edx, 1
        jne    SHORT loc_3_010B
        cmp    byte ptr [edi + ebx], 0
        je     SHORT loc_3_0182
loc_3_010B:
        push   esi
        db  068h, 0B8h, 00Bh, 000h, 000h
        db  0E8h, 0FEh, 000h, 0FAh, 0FFh
        add    esp, 8
loc_3_0119:
        or     edx, edx
        je     NEAR PTR loc_3_01D9
        push   eax
        movzx  eax, byte ptr [edi + ebx]
        push   eax
        db  068h, 0C0h, 00Bh, 000h, 000h
        db  0E8h, 0E3h, 000h, 0FAh, 0FFh
        db  083h, 0C4h
loc_3_0133:
        or     byte ptr [eax + 47h], bl
        dec    edx
        jmp    SHORT loc_3_0119
loc_3_0139:
        cmp    edx, 0c000h
        jae    SHORT loc_3_0193
        push   esi
        db  068h, 0C8h, 00Bh, 000h, 000h
        db  0E8h, 0C8h, 000h, 0FAh, 0FFh
        add    esp, 8
        test   edx, 8000h
        je     SHORT loc_3_0160
        mov    edi, dword ptr [edi + ebx]
        or     edi, edi
        je     SHORT loc_3_01CC
        sub    edi, ebx
loc_3_0160:
        and    edx, 3fffh
        je     SHORT loc_3_01CC
        cmp    byte ptr [edi + ebx], 0
        je     SHORT loc_3_01CC
        push   dword ptr [edi + ebx]
        db  068h, 0D0h, 00Bh, 000h, 000h
        db  0E8h, 099h, 000h, 0FAh, 0FFh
        add    esp, 8
        inc    edi
        dec    edx
        jmp    SHORT loc_3_0160
loc_3_0182:
        movzx  eax, al
        add    esi, eax
        dec    ecx
        je     NEAR PTR loc_3_0229
        jmp    NEAR PTR loc_3_007F
loc_3_0193:
        push   esi
        db  068h, 0D4h, 00Bh, 000h, 000h
        db  0E8h, 076h, 000h, 0FAh, 0FFh
        add    esp, 8
        movzx  eax, al
        add    esi, eax
        xor    edx, edx
loc_3_01A8:
        cmp    byte ptr [esi], 0ffh
        je     SHORT loc_3_01C9
        bt     dword ptr [edi + ebx], edx
        jae    SHORT loc_3_01C1
        push   esi
        db  068h, 0DCh, 00Bh, 000h, 000h
        db  0E8h, 056h, 000h, 0FAh, 0FFh
        add    esp, 8
loc_3_01C1:
        lodsb
        or     al, al
        jne    SHORT loc_3_01C1
        inc    edx
        jmp    SHORT loc_3_01A8
loc_3_01C9:
        inc    esi
        jmp    SHORT loc_3_01DE
loc_3_01CC:
        db  068h, 0E0h, 00Bh, 000h, 000h
        db  0E8h, 03Eh, 000h, 0FAh, 0FFh
        add    esp, 4
loc_3_01D9:
        movzx  eax, al
        add    esi, eax
loc_3_01DE:
        dec    ecx
        je     SHORT loc_3_0229
        db  0A1h, 0E4h, 00Ah, 000h, 000h
        cmp    eax, dword ptr [esp + 8]
        jae    SHORT loc_3_020D
        xor    edx, edx
        div    dword ptr [esp + 4]
        neg    edx
        add    edx, dword ptr [esp + 4]
loc_3_01F8:
        db  068h, 0E4h, 00Bh, 000h, 000h
        db  0E8h, 012h, 000h, 0FAh, 0FFh
        add    esp, 4
        dec    edx
        jne    SHORT loc_3_01F8
        jmp    NEAR PTR loc_3_007F
loc_3_020D:
        db  068h, 0E8h, 00Bh, 000h, 000h
        db  0E8h, 0FDh, 0FFh, 0F9h, 0FFh
        add    esp, 4
        db  0C7h, 005h, 0E4h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_3_007F
loc_3_0229:
        db  068h, 0ECh, 00Bh, 000h, 000h
        db  0E8h, 0E1h, 0FFh, 0F9h, 0FFh
        add    esp, 4
        add    esp, 0ch
        ret
        db  002h, 000h, 000h, 000h, 04Fh, 000h, 000h, 000h, 003h, 044h, 055h, 000h, 000h, 001h, 0FFh, 0FFh
        db  009h, 026h, 075h, 05Fh, 061h, 066h, 072h, 061h, 067h, 000h, 004h, 002h, 0FFh, 0FFh, 008h, 026h
        db  075h, 05Fh, 061h, 070h, 063h, 063h, 000h, 0D8h, 000h, 004h, 000h, 00Dh, 075h, 05Fh, 042h, 069h
        db  074h, 046h, 041h, 054h, 050h, 061h, 067h, 065h, 000h, 0AEh, 000h, 004h, 000h, 015h, 075h, 05Fh
        db  042h, 050h, 042h, 042h, 069h, 067h, 054h, 06Fh, 074h, 061h, 06Ch, 053h, 065h, 063h, 074h, 06Fh
        db  072h, 073h, 000h, 01Ch, 000h, 004h, 000h, 00Ah, 075h, 05Fh, 063h, 062h, 043h, 06Fh, 064h, 065h
        db  064h, 000h, 038h, 002h, 004h, 000h, 00Ah, 075h, 05Fh, 063h, 062h, 053h, 074h, 061h, 063h, 06Bh
        db  000h, 064h, 000h, 004h, 000h, 00Fh, 075h, 05Fh, 063h, 063h, 06Ch, 075h, 073h, 046h, 075h, 06Ch
        db  06Ch, 04Dh, 069h, 06Eh, 000h, 07Ch, 000h, 004h, 000h, 00Ch, 075h, 05Fh, 063h, 065h, 06Eh, 074h
        db  04Dh, 044h, 046h, 041h, 054h, 000h, 09Fh, 000h, 002h, 000h, 00Bh, 075h, 05Fh, 063h, 065h, 06Eh
        db  074h, 052h, 06Fh, 06Fh, 074h, 000h, 0FCh, 000h, 004h, 000h, 008h, 075h, 05Fh, 063h, 066h, 072h
        db  061h, 067h, 000h, 028h, 000h, 004h, 000h, 00Fh, 075h, 05Fh, 043h, 06Ch, 075h, 073h, 04Ch, 061h
        db  073h, 074h, 055h, 073h, 065h, 064h, 000h, 018h, 000h, 004h, 000h, 00Ch, 075h, 05Fh, 063h, 073h
        db  065h, 063h, 043h, 06Fh, 064h, 065h, 064h, 000h, 0F2h, 000h, 001h, 000h, 011h, 075h, 05Fh, 063h
        db  073h, 065h, 063h, 043h, 06Fh, 06Eh, 074h, 069h, 067h, 046h, 061h, 069h, 06Ch, 000h, 0A4h, 000h
        db  002h, 000h, 00Ah, 075h, 05Fh, 063h, 073h, 065h, 063h, 046h, 041h, 054h, 000h, 054h, 000h, 004h
        db  000h, 00Bh, 075h, 05Fh, 063h, 073h, 065h, 063h, 048h, 065h, 061h, 070h, 000h, 058h, 000h, 004h
        db  000h, 00Fh, 075h, 05Fh, 063h, 073h, 065h, 063h, 048h, 065h, 061h, 070h, 046h, 072h, 065h, 065h
        db  000h, 05Ch, 000h, 004h, 000h, 00Eh, 075h, 05Fh, 063h, 073h, 065h, 063h, 048h, 065h, 061h, 070h
        db  04Dh, 069h, 06Eh, 000h, 0CCh, 000h
loc_3_0370:
        add    al, 0
        or     esi, dword ptr [ebp + 5fh]
        arpl   word ptr [ebx + 65h], si
        arpl   word ptr [ebp + 66h], cx
        je     SHORT loc_3_037E
loc_3_037E:
        adc    al, 0
        add    al, 0
        or     al, 75h
        pop    edi
        arpl   word ptr [ebx + 65h], si
        arpl   word ptr [eax + 6ch], dx
        popad
        imul   ebp, dword ptr [esi], 2009ch
        pcmpeqw mm3, qword ptr [edi + 63h]
        jae    SHORT loc_3_03FE
        arpl   word ptr [edx + 65h], dx
        jae    SHORT loc_3_0403
        jb     SHORT loc_3_0416
        add    byte ptr fs:[eax], dl
        add    al, 0
        or     al, 75h
        pop    edi
        arpl   word ptr [ebx + 65h], si
        arpl   word ptr [edi + ebp*2 + 74h], dx
        popad
        insb
        add    al, cl
        add    byte ptr [eax + eax], al
        push   cs
        jne    SHORT loc_3_041A
        arpl   word ptr [ebx + 65h], si
        arpl   word ptr [ebp + 73h], dx
        push   ebx
        inc    edi
        inc    esp
        add    byte ptr [eax + eax + 75090002h], dh
        pop    edi
        inc    ebx
        push   esi
        inc    esi
        push   esi
        jb     NEAR PTR loc_3_03D5
loc_3_03D5:
        mov    dword ptr [eax], eax
        add    dword ptr [eax], eax
        or     dh, byte ptr [ebp + 5fh]
        inc    esp
        jb     SHORT loc_3_0448
        db  076h, 065h
        dec    esi
        outsd
        add    al, bh
        add    byte ptr [eax + eax], al
        adc    dword ptr [ebp + 5fh], esi
        inc    ebp
        inc    ebx
        push   edx
        inc    esp
        outsb
        outsd
        insd
        imul   ebp, dword ptr [esi + 61h], 726f74h
        hlt
        add    byte ptr [eax + eax], al
loc_3_03FE:
        pcmpeqw mm3, qword ptr [edi + 45h]
        inc    ebx
loc_3_0403:
        push   edx
        dec    esi
        db  075h, 06Dh
        jb     NEAR PTR loc_3_046B
        db  074h, 06Fh
        jb     SHORT loc_3_040E
loc_3_040E:
        js     SHORT loc_3_0410
loc_3_0410:
        add    al, 0
        adc    dh, byte ptr [ebp + 5fh]
        inc    esi
loc_3_0416:
        inc    ecx
        push   esp
        je     SHORT loc_3_0489
loc_3_041A:
        dec    ebp
        inc    esp
        inc    esi
        inc    ecx
        push   esp
        inc    esp
        insb
        je     SHORT loc_3_0485
        add    byte ptr [eax + eax], al
        add    al, 0c0h
        or     byte ptr [ebp + 5fh], dh
        inc    esi
        insb
        popad
        jae    NEAR PTR loc_3_0432
loc_3_0432:
        push   ebx
        push   edi
        push   eax
        add    byte ptr [esi + 31h], al
        xor    al, byte ptr [eax]
        push   edx
        inc    ebp
        dec    ebp
        add    byte ptr [eax + 4dh], dl
        dec    edi
        add    byte ptr [edx + 41h], al
        inc    esp
        add    byte ptr [ebx + 48h], al
loc_3_0448:
        inc    esp
        add    byte ptr [ebx + 4ch], al
        inc    esp
        add    byte ptr [ebx + 44h], al
        push   edx
        add    byte ptr [ebx + 44h], al
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [edi + 52h], dl
        inc    esp
        add    byte ptr [edi + 52h], dl
        dec    ebp
        add    byte ptr [edx + 49h], al
        push   esp
        add    byte ptr [eax], al
        inc    esi
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
        inc    ebx
loc_3_046B:
        dec    esp
        dec    esi
        add    bh, bh
        add    al, 0
        add    al, 0
        or     byte ptr [ebp + 5fh], dh
        inc    esi
        insb
        popad
        jae    NEAR PTR loc_3_047C
loc_3_047C:
        dec    esp
        add    byte ptr [eax + eax], al
        adc    dh, byte ptr [ebp + 5fh]
        dec    ecx
        outsb
loc_3_0485:
        je     SHORT loc_3_04EC
        jb     SHORT loc_3_04F7
loc_3_0489:
        popad
        insb
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_3_0493
loc_3_0493:
        fadd   qword ptr [eax]
        add    al, 0
        or     esi, dword ptr [ebp + 5fh]
        dec    ecx
        dec    edi
        push   eax
        inc    esi
        insb
        popad
        jae    NEAR PTR loc_3_04A3
loc_3_04A3:
        lock add byte ptr [edx], al
        add    byte ptr [ebx], cl
        jne    SHORT loc_3_0509
        dec    ecx
        dec    edi
        push   ebx
        je     SHORT loc_3_0510
        je     SHORT loc_3_0526
        jae    SHORT loc_3_04B3
loc_3_04B3:
        add    byte ptr [eax], 4
        add    byte ptr [edi], cl
        db  075h, 05Fh
        imul   esi, dword ptr [eax + 67h], 42727543h
        imul   esi, dword ptr [esi + eax*2 + 41h], 840054h
        add    al, 0
        adc    byte ptr [ebp + 5fh], dh
        imul   esi, dword ptr [eax + 67h], 7473614ch
        inc    edx
        imul   esi, dword ptr [esi + eax*2 + 41h], 600054h
        add    al, 0
        or     esi, dword ptr [ebp + 5fh]
        imul   esi, dword ptr [ebx + 65h], 61654863h
        jo     SHORT loc_3_04EC
loc_3_04EC:
        mov    word ptr [eax], es
        add    dword ptr [eax], eax
        adc    byte ptr [ebp + 5fh], dh
        insb
        outsd
        push   ebx
loc_3_04F7:
        arpl   word ptr gs:[eax + 65h], dx
        db  072h, 043h
        insb
        jne    SHORT loc_3_0573
        add    byte ptr [ebx + 14000100h], cl
        db  075h, 05Fh
        insb
loc_3_0509:
        outsd
        dec    eax
        outsd
        db  073h, 074h
        push   ebx
loc_3_0510:
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        push   ebx
        imul   edi, dword ptr [edx + 65h], 1001100h
        add    byte ptr [esi], cl
        db  075h, 05Fh
        insd
        arpl   word ptr fs:[ebx + 65h], si
loc_3_0526:
        arpl   word ptr [ebx + 6fh], sp
        add    byte ptr fs:[eax], dl
        add    byte ptr [ecx], al
        add    byte ptr [esi], cl
        db  075h, 05Fh
        insd
        arpl   word ptr fs:[ebx + 65h], si
        arpl   word ptr [eax + 6ch], si
        popad
        imul   ebp, dword ptr [esi], 40008h
        or     eax, 444d5f75h
        inc    esi
        inc    ecx
        push   esp
        dec    ecx
        outsb
        js     NEAR PTR loc_3_0552
loc_3_0552:
        sldt   word ptr [ecx]
        ror    byte ptr [edx], 75h
        pop    edi
        insd
        insb
        popad
        jae    NEAR PTR loc_3_0561
loc_3_0561:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 52h], al
        inc    edi
        add    byte ptr [ebp + 4eh], dl
        inc    ebx
        add    byte ptr [ecx + 4ch], al
        dec    esp
        add    bh, bh
loc_3_0573:
        or     al, 0
        add    al, 0
        or     eax, 646d5f75h
        jae    SHORT loc_3_05E3
        arpl   word ptr [esi + 6ch], sp
        popad
        jae    NEAR PTR loc_3_0585
loc_3_0585:
        lea    eax, [eax]
        add    dword ptr [eax], eax
        adc    byte ptr [ebp + 5fh], dh
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 44h], 72637365h
        imul   esi, dword ptr [eax + 74h], 4002000h
        add    byte ptr [edx], cl
        db  075h, 05Fh
        outsd
        inc    sp
        popad
        je     SHORT loc_3_0609
        add    byte ptr [eax + eax], ah
        add    al, 0
        or     al, 75h
        pop    edi
        db  070h, 062h
        inc    ebx
        insb
        db  075h, 073h
        db  074h, 065h
        jb     SHORT loc_3_05BA
loc_3_05BA:
        cmp    al, 2
        add    al, 0
        or     dh, byte ptr [ebp + 5fh]
        db  070h, 062h
        push   ebx
        je     SHORT loc_3_0627
        arpl   word ptr [ebx], bp
        sub    al, 0
        add    al, 0
        or     al, 75h
        pop    edi
        jo     SHORT loc_3_0635
        popad
        inc    edx
        imul   esi, dword ptr [esi + eax*2 + 41h], 0e40054h
        add    al, 0
        or     esi, dword ptr [ebp + 5fh]
        db  070h, 063h
loc_3_05E3:
        popad
        inc    esi
        insb
        jne    SHORT loc_3_065B
        push   4003000h
        add    byte ptr [ebx], cl
        db  075h, 05Fh
        jo     SHORT loc_3_0656
        popad
        dec    ebp
        inc    esp
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [eax], bh
        add    byte ptr [eax + eax], al
        or     esi, dword ptr [ebp + 5fh]
        db  070h, 064h
        arpl   word ptr [edx + 48h], sp
        outsd
        jae    SHORT loc_3_067D
loc_3_0609:
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        or     dh, byte ptr [ebp + 5fh]
        jo     SHORT loc_3_0677
        db  075h, 04Eh
        js     NEAR PTR loc_3_068C
        add    byte ptr [edx + eax + 4], al
        add    byte ptr [edi], cl
        db  075h, 05Fh
        db  070h, 064h
        db  075h, 04Eh
        db  065h, 078h, 074h
loc_3_0627:
        push   ecx
        db  075h, 065h
        jne    SHORT loc_3_0691
        add    byte ptr [esi + 0f000200h], dh
        jne    SHORT loc_3_0693
        push   eax
loc_3_0635:
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 6f727245h
        jb     SHORT loc_3_0641
loc_3_0641:
        xor    al, 0
        add    al, 0
        or     al, 75h
        pop    edi
        jo     SHORT loc_3_06B0
        outsb
        inc    esp
        imul   esi, dword ptr [ebx + 6bh], 48004f49h
        add    byte ptr [eax + eax], al
loc_3_0656:
        or     esi, dword ptr [ebp + 5fh]
        jo     SHORT loc_3_06C4
loc_3_065B:
        outsd
        jo     SHORT loc_3_06AD
        db  072h, 069h
        add    byte ptr [bx + si + 2], al
        add    al, 0
        or     esi, dword ptr [ebp + 5fh]
        db  070h, 069h
        outsd
        db  070h, 057h
        outsd
        db  072h, 06Bh
        add    byte ptr [esi + 0cffff00h], cl
loc_3_0677:
        jne    NEAR PTR loc_3_06D9
        push   eax
        insb
        popad
loc_3_067D:
        imul   ebp, dword ptr [esi + 42h], 0b8004250h
        add    byte ptr [eax + eax], al
        or     dword ptr [ebp + 5fh], esi
        db  070h, 072h
loc_3_068C:
        insd
        pop    edi
        jne    NEAR PTR loc_3_0691
loc_3_0691:
        rol    byte ptr [eax], 1
loc_3_0693:
        add    al, 0
        or     dh, byte ptr [ebp + 5fh]
        jo     SHORT loc_3_070D
        inc    ebx
        db  075h, 072h
        add    byte ptr [eax + eax], bh
        add    al, 0
        pop    es
        db  075h, 05Fh
        db  070h, 076h
        db  072h, 070h
        add    al, ah
loc_3_06AD:
        add    byte ptr [eax + eax], al
loc_3_06B0:
        or     esi, dword ptr [ebp + 5fh]
        jo     SHORT loc_3_072C
        push   ebx
        db  074h, 061h
        db  074h, 075h
        jae    SHORT loc_3_06BC
loc_3_06BC:
        mov    bl, 0
        add    dword ptr [eax], eax
        or     byte ptr [ebp + 5fh], dh
        push   edx
loc_3_06C4:
        db  065h, 071h, 049h
        inc    esp
        add    byte ptr [eax + eax + 4], dh
        add    byte ptr [ebx], cl
        db  075h, 05Fh
        db  073h, 065h
        arpl   word ptr [ebx + 6ch], ax
        jne    SHORT loc_3_074A
        xor    al, byte ptr [eax]
loc_3_06D9:
        aam    0
        add    al, 0
        or     dword ptr [ebp + 5fh], esi
        db  073h, 065h
        arpl   word ptr [ebx + 75h], ax
        jb     SHORT loc_3_06E7
loc_3_06E7:
        insb
        add    byte ptr [eax + eax], al
        or     dh, byte ptr [ebp + 5fh]
        jae    SHORT loc_3_0755
        arpl   word ptr [eax + 65h], cx
        popad
        jo     SHORT loc_3_06F6
loc_3_06F6:
        push   0b000400h
        db  075h, 05Fh
        db  073h, 065h
        arpl   word ptr [ebp + 44h], cx
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [ebx + 0d000100h], bl
        db  075h, 05Fh
loc_3_070D:
        push   ebx
        arpl   word ptr gs:[eax + 65h], dx
        db  072h, 043h
        insb
        jne    SHORT loc_3_078A
        add    byte ptr [eax], dh
        add    al, 0
        pcmpeqw mm3, qword ptr [edi + 73h]
        arpl   word ptr gs:[edx + 6fh], dx
        outsd
        je     SHORT loc_3_0768
        jo     SHORT loc_3_0799
        popad
        jb     SHORT loc_3_072C
loc_3_072C:
        mov    esp, 0c000400h
        db  075h, 05Fh
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        dec    ebp
        popad
        jo     SHORT loc_3_073D
loc_3_073D:
        rol    byte ptr [eax], 4
        add    byte ptr [edi], cl
        jne    SHORT loc_3_07A3
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
loc_3_074A:
        dec    ebp
        popad
        db  070h, 045h
        outsb
        add    ah, al
        add    byte ptr [eax + eax], al
loc_3_0755:
        adc    dh, byte ptr [ebp + 5fh]
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        dec    ebp
        popad
        jo     SHORT loc_3_07A7
        outsb
        dec    ebp
        popad
        js     SHORT loc_3_0768
loc_3_0768:
        cdq
        add    byte ptr [edx], al
        add    byte ptr [65535f75h], cl
        arpl   word ptr [edi + ebp*2 + 72h], si
        push   ebx
        imul   edi, dword ptr [edx + 65h], 100b200h
        add    byte ptr [65535f75h], cl
        jno    SHORT loc_3_07FA
        outsb
        arpl   word ptr [ebp + 4eh], sp
loc_3_078A:
        outsd
        add    byte ptr [eax + 2], cl
        add    al, 40h
        or     eax, 74535f75h
        popad
        arpl   word ptr [ebx + 46h], bp
loc_3_0799:
        outsb
        arpl   word ptr [ebp], sp
        dec    esp
        add    bh, bh
        dec    dword ptr [ecx]
loc_3_07A3:
        db  026h, 075h, 05Fh
        push   ebx
loc_3_07A7:
        je     SHORT loc_3_080A
        arpl   word ptr [ebx], bp
loc_3_07AC:
        mov    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     al, 75h
        pop    edi
        push   esp
        push   72447369h
        imul   esi, dword ptr [esi + 65h], 1008a00h
        add    byte ptr [ebx], cl
        jne    SHORT loc_3_0823
        push   esp
        db  072h, 075h
        dec    eax
        outsd
        jae    SHORT loc_3_0840
        add    byte ptr [eax], cl
        add    al, 0
        adc    esi, dword ptr [ebp + 5fh]
loc_3_07D4:
        push   ebp
        outsb
        imul   esi, dword ptr [ebx + edx*2 + 65h], 6c772e6dh
loc_3_07DE:
        pop    edi
        outsd
        ja     SHORT loc_3_0850
        jb     NEAR PTR loc_3_07E5
loc_3_07E5:
        inc    eax
        add    byte ptr [eax + eax], al
        push   ss
        db  075h, 05Fh
        push   ebp
        outsb
        imul   esi, dword ptr [ebx + edx*2 + 65h], 6c772e6dh
        pop    edi
        jo     SHORT loc_3_0862
loc_3_07F9:
        outsd
loc_3_07FA:
        jo     SHORT loc_3_0864
        popad
        add    byte ptr fs:[edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        push   es
        push   ebx
loc_3_080A:
        push   esp
        inc    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        or     eax, 5f617473h
        inc    edx
        imul   esp, dword ptr [edi + 52h], 73646165h
        add    byte ptr [eax + eax], al
loc_3_0823:
        add    al, 0
        adc    esi, dword ptr [ebx + 74h]
        popad
        pop    edi
        inc    edx
        imul   esp, dword ptr [edi + 52h], 53646165h
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        jae    SHORT loc_3_0839
loc_3_0839:
        or     byte ptr [eax], al
        add    al, 0
        adc    byte ptr [ebx + 74h], dh
loc_3_0840:
        popad
        pop    edi
        inc    edx
        jne    SHORT loc_3_08AE
        insb
        inc    edx
        imul   esi, dword ptr [esi + eax*2 + 41h], 0c0054h
loc_3_0850:
        add    al, 0
        db  00Fh, 073h, 074h, 061h, 05Fh, 043h, 061h, 063h
loc_3_085A:
        push   756f4665h
        outsb
        db  064h, 000h
loc_3_0862:
        adc    byte ptr [eax], al
loc_3_0864:
        add    al, 0
        adc    byte ptr [ebx + 74h], dh
        popad
loc_3_086A:
        pop    edi
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        dec    eax
        imul   ebp, dword ptr [esi + 74h], 14004b4fh
        add    byte ptr [eax + eax], al
        adc    eax, 5f617473h
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        dec    eax
        imul   ebp, dword ptr [esi + 74h], 63736944h
        popad
        jb     SHORT loc_3_08F4
        add    byte ptr [eax], bl
        add    byte ptr [eax + eax], al
        push   cs
        jae    SHORT loc_3_090C
        popad
        pop    edi
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        push   edx
        popad
        add    byte ptr fs:[eax + eax], bl
        add    al, 0
        push   cs
        jae    SHORT loc_3_091F
        popad
        pop    edi
        inc    ebx
loc_3_08AE:
        popad
        arpl   word ptr [eax + 65h], bp
        push   edi
        popad
        imul   esi, dword ptr [eax + eax + 20h], 0f000400h
        db  073h, 074h
        popad
        pop    edi
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        push   edi
        db  072h, 069h
        db  074h, 065h
        add    byte ptr [eax], ch
        add    byte ptr [eax + eax], al
        db  00Fh
loc_3_08D0:
        db  073h, 074h
        popad
        pop    edi
        inc    ebx
        push   526b6365h
        db  065h, 075h, 073h
        add    byte ptr gs:[eax + eax], ch
        add    al, 0
        db  00Fh, 073h, 074h, 061h, 05Fh, 043h, 06Fh, 06Dh, 070h, 044h, 069h, 072h, 065h, 063h, 074h, 000h
        db  030h
loc_3_08F4:
        add    byte ptr [eax + eax], al
        or     eax, 5f617473h
        inc    ebx
        outsd
        insd
        db  070h, 072h
        db  065h, 073h, 073h
        add    byte ptr [eax + eax], dh
        add    al, 0
        db  00Dh, 073h, 074h
loc_3_090C:
        popad
        pop    edi
        inc    ebx
        outsd
        insd
        db  070h, 053h
        popad
        db  076h, 065h
        add    byte ptr [eax], bh
        add    byte ptr [eax + eax], al
        db  00Fh, 073h, 074h, 061h
loc_3_091F:
        pop    edi
        arpl   word ptr [ebx + 65h], si
        db  063h
_ITEXT ends
        end
