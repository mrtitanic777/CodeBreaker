; pppmac_pcode_02.asm -- PPPMAC object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  093h, 000h, 000h, 000h
loc_2_0004:
        xor    al, 1
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        and    al, byte ptr [eax]
loc_2_0010:
        inc    esi
        add    byte ptr [ebx], bl
loc_2_0014:
        cdq
        add    ah, cl
        add    byte ptr [edi], al
        add    dword ptr [edi], ebp
        add    dword ptr [edx], edi
        add    dword ptr [eax + 1], ebx
loc_2_0020:
        db  074h, 001h
loc_2_0022:
        lcall  6102h, 1f01d901h
        add    dh, byte ptr [eax + 2102e802h]
        add    eax, dword ptr [ebp + 3]
        outsb
        add    eax, dword ptr [ebx - 33fc5efdh]
        add    esi, ecx
        add    ebp, dword ptr [edx]
        add    al, 73h
        add    al, 98h
        add    al, 0c1h
        add    al, 0e9h
        add    al, 13h
        add    eax, 5770548h
loc_2_004C:
        mov    ch, 5
loc_2_004E:
        add    eax, 64e0620h
loc_2_0054:
        mov    es, word ptr [esi]
loc_2_0056:
        aad    6
loc_2_0058:
        or     eax, 6d074407h
        pop    es
loc_2_005E:
        xchg   ecx, eax
        pop    es
loc_2_0060:
        mov    esp, 7dd07h
        or     byte ptr [ebx], ch
        or     byte ptr [ebp + 8], ah
loc_2_006A:
        stosd
        or     bl, dh
        or     byte ptr [edx], bh
        or     dword ptr [ecx - 12f644f7h], eax
        or     dword ptr [edx + ecx], esp
loc_2_0078:
        inc    esp
        or     ah, byte ptr [esi + 0ah]
loc_2_007C:
        pushfd
        or     dl, dh
        or     al, byte ptr [ebx + ecx]
loc_2_0082:
        inc    ebp
        or     esi, dword ptr [esi + 0bh]
loc_2_0086:
        scasb
        or     esp, esp
        or     ebx, dword ptr [ebx]
        or     al, 49h
        or     al, 78h
        or     al, 0a3h
        or     al, 0cah
        or     al, 0f4h
        or     al, 37h
        or     eax, 0dae0d7ah
loc_2_009C:
        db  0EBh, 00Dh
loc_2_009E:
        and    cl, byte ptr [esi]
loc_2_00A0:
        inc    ebp
        push   cs
loc_2_00A2:
        jae    SHORT loc_2_00B2
loc_2_00A4:
        mov    al, 0eh
loc_2_00A6:
        jmp    SHORT loc_2_00B6
loc_2_00A8:
        and    byte ptr [edi], cl
loc_2_00AA:
        cmp    cl, byte ptr [edi]
loc_2_00AC:
        db  073h, 00Fh
loc_2_00AE:
        mov    ch, 0fh
loc_2_00B0:
        in     eax, 0fh
loc_2_00B2:
        adc    byte ptr [eax], dl
        db  036h, 010h
loc_2_00B6:
        pop    eax
        db  010h
loc_2_00B8:
        db  07Bh, 010h
loc_2_00BA:
        pushfd
        db  010h, 0BEh, 010h
loc_2_00BE:
        fist   word ptr [eax]
loc_2_00C0:
        add    edx, dword ptr [ecx]
loc_2_00C2:
        sub    byte ptr [ecx], dl
loc_2_00C4:
        dec    ebx
        adc    dword ptr [esi + 11h], esi
loc_2_00C8:
        lodsb
        adc    esi, esi
        adc    dword ptr [edx], edx
        db  012h
loc_2_00CE:
        adc    ch, byte ptr ss:[eax + 12h]
loc_2_00D2:
        xchg   ebx, eax
        db  012h
loc_2_00D4:
        mov    dh, 12h
loc_2_00D6:
        in     al, 12h
loc_2_00D8:
        pop    ss
        db  013h
loc_2_00DA:
        cmp    al, 13h
        db  082h, 013h
loc_2_00DE:
        lds    edx, ptr [ebx]
loc_2_00E0:
        db  0E2h, 013h
loc_2_00E2:
        sbb    edx, dword ptr [edx]
        adc    al, 80h
        adc    al, 0c3h
        adc    al, 9
        adc    eax, 1594154ah
loc_2_00F0:
        movsb
        adc    eax, 15ec15c2h
loc_2_00F6:
        or     byte ptr [esi], dl
loc_2_00F8:
        cmp    eax, 57165316h
        push   ss
loc_2_00FE:
        pop    esp
        push   ss
loc_2_0100:
        bound  edx, qword ptr [esi]
loc_2_0102:
        push   71166c16h
        push   ss
loc_2_0108:
        jne    SHORT loc_2_0120
        adc    byte ptr [esi], 8ch
        push   ss
        db  08Fh, 016h
loc_2_0110:
        xchg   esp, eax
        push   ss
loc_2_0112:
        pushfd
        push   ss
        movsd
        push   ss
loc_2_0116:
        mov    al, 16h
loc_2_0118:
        mov    edi, 0f516da16h
        push   ss
loc_2_011E:
        adc    byte ptr [edi], dl
loc_2_0120:
        sub    edx, dword ptr [edi]
loc_2_0122:
        inc    esi
        pop    ss
loc_2_0124:
        popad
        pop    ss
loc_2_0126:
        db  07Ch, 017h
loc_2_0128:
        xchg   edi, eax
        pop    ss
loc_2_012A:
        mov    dl, 17h
loc_2_012C:
        int    17h
loc_2_012E:
        db  0E8h, 017h, 009h, 018h, 02Dh
        db  018h
loc_2_0134:
        push   edx
        insd
        outsd
        db  074h, 065h
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_2_0162
        db  064h, 072h, 069h
        jbe    SHORT loc_2_01AC
        jb     SHORT loc_2_0169
        insb
        outsd
        and    byte ptr [bx + 70h], ch
        outsb
        or     al, byte ptr cs:[eax]
        push   edx
        insd
        outsd
        je     SHORT loc_2_01C1
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
loc_2_0162:
        db  073h, 020h
        jb     NEAR PTR loc_2_01D0
        jbe    SHORT loc_2_01CE
loc_2_0169:
        db  072h, 020h
        insb
        outsd
        and    byte ptr [bp + di + 6ch], ah
        outsd
        db  073h, 065h
        or     al, byte ptr cs:[eax]
        or     al, byte ptr [eax]
        push   ebx
        jb     NEAR PTR loc_2_01F4
        jb     NEAR PTR loc_2_01A1
        db  074h, 079h
        db  070h, 065h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [eax], ah
        and    eax, 0a2e73h
        dec    esi
        inc    ebp
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        and    byte ptr [ebp + 75h], ch
        db  073h, 074h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [edx + 6fh], ah
loc_2_01A1:
        db  075h, 06Eh
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6eh], bp
        outsb
loc_2_01AC:
        arpl   word ptr gs:[eax + 74h], si
        outsd
        and    byte ptr [ecx + 20h], ah
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        dec    esi
        push   esp
        and    byte ptr [ebx], dh
loc_2_01C1:
        xor    dword ptr cs:[eax], esp
        push   ebx
        jb     NEAR PTR loc_2_023E
        jb     NEAR PTR loc_2_01F9
        or     al, byte ptr [eax]
        push   esp
loc_2_01CE:
        inc    ebx
        push   eax
loc_2_01D0:
        das
        dec    ecx
        push   eax
        and    byte ptr [ebp + 75h], ch
        jae    SHORT loc_2_024C
        and    byte ptr [edx + 65h], ah
        and    byte ptr [edx + 6fh], ah
        jne    SHORT loc_2_024E
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[eax + 74h], si
        outsd
        and    byte ptr [ecx + 20h], ah
        push   ebx
        dec    esp
loc_2_01F4:
        dec    ecx
        push   eax
        and    byte ptr [ebx + 65h], dl
loc_2_01F9:
        db  072h, 076h
        jb     NEAR PTR loc_2_022C
        or     al, byte ptr [eax]
        dec    ecx
        push   eax
        pop    eax
        and    byte ptr [ebp + 75h], ch
        jae    SHORT loc_2_027C
        and    byte ptr [edx + 65h], ah
        and    byte ptr [edx + 6fh], ah
        db  075h, 06Eh
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[eax + 74h], si
        outsd
        and    byte ptr [ecx + 20h], ah
        dec    esi
        db  065h, 074h, 077h
        popad
        jb     SHORT loc_2_028E
        and    byte ptr [ebx + 6fh], al
loc_2_022C:
        outsb
        outsb
        arpl   word ptr gs:[eax + 53h], si
        db  065h, 072h, 076h
        db  065h, 072h, 02Eh
        or     al, byte ptr [eax]
        push   edx
        insd
loc_2_023E:
        outsd
        db  074h, 065h
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        jb     NEAR PTR loc_2_02B5
loc_2_024C:
        db  076h, 065h
loc_2_024E:
        jb     SHORT loc_2_0270
        imul   esi, dword ptr [ebx + 20h], 74756873h
        db  074h, 069h
        outsb
        and    byte ptr [si + 6fh], ah
        ja     SHORT loc_2_02CE
        or     al, byte ptr cs:[eax]
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [0a2e73h], ah
        push   ebx
        outsd
loc_2_0270:
        db  066h, 074h, 077h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_02EE
loc_2_027C:
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 73696420h
        popad
        bound  ebp, qword ptr [ebp + 64h]
        add    byte ptr [ebp + 6eh], al
loc_2_028E:
        arpl   word ptr [edx + 79h], si
        db  070h, 074h
        and    byte ptr fs:[eax + 61h], dl
        jae    SHORT loc_2_030D
        db  077h, 06Fh
        jb     SHORT loc_2_0302
        and    byte ptr [edx + 65h], dh
        jno    SHORT loc_2_0318
        imul   esi, dword ptr [edx + 65h], 73550064h
        db  065h, 072h, 020h
        insb
        jbe    NEAR PTR loc_2_0316
        insb
        and    byte ptr [ebx + 65h], dh
loc_2_02B5:
        arpl   word ptr [ebp + 72h], si
        imul   esi, dword ptr [ecx + edi*2 + 20h], 4e206f74h
        db  065h, 074h, 077h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 65h], dl
        db  072h, 076h
        jb     NEAR PTR loc_2_02CE
loc_2_02CE:
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [eax + 72h], dl
        outsd
        je     SHORT loc_2_0348
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        bound  ebp, qword ptr [edi + 75h]
        outsb
        and    byte ptr fs:[696b7320h], ch
        db  070h, 070h
loc_2_02EE:
        imul   ebp, dword ptr [esi + 67h], 6e6f6320h
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
loc_2_0302:
        and    byte ptr [25282078h], ah
        db  073h, 029h
        or     al, byte ptr cs:[eax]
loc_2_030D:
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [eax + 72h], dl
        outsd
loc_2_0316:
        je     SHORT loc_2_0387
loc_2_0318:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ebp + 64h]
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ebp + 73h], dh
        db  065h, 072h, 020h
        sub    eax, 696b7320h
        jo     SHORT loc_2_03A4
        imul   ebp, dword ptr [esi + 67h], 6e6f6320h
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
loc_2_0348:
        and    byte ptr [25282078h], ah
        db  073h, 029h
        or     al, byte ptr cs:[eax]
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 64h], al
        imul   ebp, dword ptr fs:[esi + 67h], 6e6f4320h
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 72h], dl
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [25282078h], ah
        jae    SHORT loc_2_03A1
        and    byte ptr [edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
loc_2_0387:
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [ebx + 68h], ah
        popad
        imul   ebp, dword ptr [esi + 2eh], 5346000ah
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [esi + 65h], cl
        outsd
        db  074h, 069h
loc_2_03A1:
        popad
        je     SHORT loc_2_040D
loc_2_03A4:
        outsd
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 63202d20h
        outsd
        outsb
        je     SHORT loc_2_0427
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [25282078h], ah
        db  073h, 029h
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 0a2e64h
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 65h], dl
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 6f727020h
        je     SHORT loc_2_0466
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
loc_2_040D:
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [0a2e78h], ah
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
loc_2_0427:
        db  076h, 065h
        and    byte ptr fs:[eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_2_04B6
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [0a2e78h], ah
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 73h], cl
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_2_04D7
        outsd
loc_2_0466:
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ebp + 70h], dh
        or     al, byte ptr cs:[eax]
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [esi + 6fh], cl
        and    byte ptr [esi + 65h], ch
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_2_0502
        arpl   word ptr [edi + 6ch], bp
        db  073h, 020h
        jae    SHORT loc_2_050E
        popad
        jb     SHORT loc_2_0511
        or     al, byte ptr cs:[eax]
        inc    esi
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [ebx + eax*2 + 50h], cl
        and    byte ptr [esi + 69h], al
        outsb
        imul   esi, dword ptr [ebx + 68h], 0a2e6465h
loc_2_04B6:
        add    byte ptr [esi + 53h], al
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 75h], al
        db  074h, 068h
        outsb
        db  074h, 069h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 69616620h
        insb
        or     al, byte ptr cs:[eax]
        inc    esi
        push   ebx
loc_2_04D7:
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [eax + 72h], dl
        db  065h, 070h, 061h
        db  072h, 069h
        outsb
        and    byte ptr [bp + 6fh], ah
        jb     SHORT loc_2_0509
        arpl   word ptr [ecx + 6ch], sp
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 66h
        db  072h, 06Fh
        insd
        and    byte ptr [ebx + 65h], dh
        jb     SHORT loc_2_0571
        db  065h, 072h, 02Eh
        or     al, byte ptr [eax]
        inc    esi
        push   ebx
loc_2_0502:
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 73h], cl
loc_2_0509:
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
loc_2_050E:
        db  074h, 072h
        outsd
loc_2_0511:
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 46000a2eh
        push   ebx
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [esi + 6fh], cl
        and    byte ptr [esi + 65h], ch
        je     SHORT loc_2_05A9
        outsd
        db  072h, 06Bh
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_2_05AA
        arpl   word ptr [edi + 6ch], bp
        jae    SHORT loc_2_0560
        ja     SHORT loc_2_05A7
        jb     SHORT loc_2_05A9
        and    byte ptr [ebx + 75h], dh
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_2_05BF
        db  066h, 075h, 06Ch
        jns    SHORT loc_2_0571
        outsb
        outsd
        je     SHORT loc_2_05C0
        popad
        je     SHORT loc_2_05BF
        or     al, byte ptr cs:[eax]
        inc    esi
        push   ebx
loc_2_0560:
        inc    ecx
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        insd
        outsd
        je     SHORT loc_2_05CF
        and    byte ptr [eax + 50h], dl
        push   eax
        and    byte ptr [ebx + 6fh], dh
loc_2_0571:
        je     NEAR PTR loc_2_05EB
        popad
        db  072h, 065h
        and    byte ptr [ebx + 65h], dh
        outsb
        db  074h, 020h
        db  075h, 06Eh
        db  072h, 065h
        arpl   word ptr [edi + 67h], bp
        outsb
        imul   edi, dword ptr [edx + 65h], 61702064h
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_05B1
        outsd
        and    byte ptr [ecx + edi*2 + 70h], dh
        and    byte ptr gs:[6f742078h], ah
        and    byte ptr [ebx + 50h], al
        and    byte ptr [0a2178h], ah
loc_2_05A7:
        dec    esp
        inc    ebx
loc_2_05A9:
        push   eax
loc_2_05AA:
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 61h], al
        insb
        insb
loc_2_05B1:
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 6eh
        outsd
        je     SHORT loc_2_0625
        popad
        je     SHORT loc_2_0628
loc_2_05BF:
        outsd
loc_2_05C0:
        outsb
        and    byte ptr [ebp + 6eh], ah
        popad
        bound  ebp, qword ptr [ebp + 64h]
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
loc_2_05CF:
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        and    byte ptr gs:[edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
loc_2_05EB:
        arpl   word ptr [edi + 64h], bp
        and    byte ptr gs:[0a2e78h], ah
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ebp + 52h], cl
        push   ebp
        and    byte ptr [edi + 66h], ch
        and    byte ptr [0a2e64h], ah
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
loc_2_0625:
        arpl   word ptr [ebp + 69h], sp
loc_2_0628:
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_069C
        and    byte ptr fs:[ecx + 43h], al
        inc    ebx
        dec    ebp
        and    byte ptr [edi + 66h], ch
        and    byte ptr [0a2e786ch], ah
        add    byte ptr [ebx + eax*2 + 50h], cl
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6eh], si
        and    byte ptr [bx + di + 75h], ah
        je     SHORT loc_2_06C3
        outsb
        db  074h, 069h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 71657220h
        db  075h, 065h
        db  073h, 074h
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [ebx + 6ch], ah
        imul   esp, dword ptr [ebp + 6eh], 0a2e74h
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 6fh], cl
        and    byte ptr [ecx + 75h], ah
        je     SHORT loc_2_06F1
        outsb
        db  074h, 069h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
loc_2_069C:
        db  073h, 020h
        arpl   word ptr [edi + 6eh], bp
        imul   sp, word ptr [edi + 75h], 6572h
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
loc_2_06C3:
        db  074h, 065h
        and    byte ptr fs:[ecx + 75h], ah
        je     SHORT loc_2_0733
        outsb
        db  074h, 069h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [25282078h], ah
        db  073h, 029h
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
loc_2_06F1:
        dec    ebx
        and    byte ptr [ecx + 75h], ah
        db  074h, 068h
        outsb
        db  074h, 069h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [69772078h], ah
        db  074h, 068h
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_2_0787
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [25282078h], ah
        jae    SHORT loc_2_074C
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
loc_2_0733:
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ebp + 61h], ch
        imul   esp, dword ptr [bp + di + 20h], 626d756eh
loc_2_074C:
        jb     NEAR PTR loc_2_076F
        and    eax, 0a2e78h
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 61h], cl
        imul   ebp, dword ptr [ecx + 6eh], 67h
        and    byte ptr [eax + 6fh], dh
        db  073h, 073h
        imul   esp, dword ptr [edx + 6ch], 6f6c2079h
        outsd
        db  070h, 062h
loc_2_076F:
        popad
        arpl   word ptr [ebx + 20h], bp
        insd
        popad
        imul   esp, dword ptr [bp + di + 20h], 626d756eh
        db  065h, 072h, 02Eh
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
loc_2_0787:
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[eax + 72h], dh
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        db  070h, 072h
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0834
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_0841
        and    byte ptr fs:[ecx + 64h], ah
        db  064h, 072h, 065h
        db  073h, 073h
        sub    esp, dword ptr [ebx + 6fh]
        outsb
        db  074h, 072h
        outsd
        insb
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        jo     SHORT loc_2_086B
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6eh], si
        and    byte ptr [bp + di + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 70h
        db  072h, 06Fh
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 65h], ch
        outsd
        db  074h, 069h
        popad
        db  074h, 069h
loc_2_0834:
        outsd
        outsb
        and    byte ptr [edx + 65h], dh
        db  071h, 075h
        db  065h, 073h, 074h
        or     al, byte ptr cs:[eax]
loc_2_0841:
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_08B3
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 70h
        db  072h, 06Fh
        db  074h, 06Fh
loc_2_086B:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
        dec    ebx
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 70h
        jb     SHORT loc_2_08FD
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [69772078h], ah
        db  074h, 068h
        and    byte ptr [0a2e78h], ah
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6eh], si
        and    byte ptr [di + 6eh], dh
loc_2_08B3:
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [0a2e64h], ah
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + esi*2 + 69h], dl
        and    byte ptr fs:[ecx + 6ch], ah
        insb
        and    byte ptr [ecx + 75h], ah
        je     SHORT loc_2_0941
        outsb
        je     SHORT loc_2_0946
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        jae    SHORT loc_2_091C
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0962
loc_2_08FD:
        and    byte ptr fs:[ebp + 6eh], dh
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [ebx + 6fh], ah
        and    byte ptr gs:[0a2e64h], ah
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
loc_2_091C:
        db  076h, 065h
        and    byte ptr fs:[ebp + 72h], dh
        insd
        imul   ebp, dword ptr [esi + 61h], 72206574h
        db  065h, 071h, 075h
        db  065h, 073h, 074h
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_09A6
loc_2_0941:
        and    byte ptr fs:[ebp + 72h], dh
loc_2_0946:
        insd
        imul   ebp, dword ptr [esi + 61h], 61206574h
        arpl   word ptr [ebx + 6eh], bp
        outsd
        ja     SHORT loc_2_09C0
        insd
        outsb
        je     SHORT loc_2_098B
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
        push   eax
loc_2_0962:
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_09D1
        and    byte ptr fs:[ebx + 6fh], ah
        jb     SHORT loc_2_09E4
        jne    SHORT loc_2_09E4
        db  074h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
loc_2_098B:
        db  072h, 020h
        outsd
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 2e642520h
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0A0B
loc_2_09A6:
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        popad
        jne    SHORT loc_2_0A34
loc_2_09C0:
        push   69746e65h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        db  074h, 06Fh
loc_2_09D1:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [25282078h], ah
        jae    SHORT loc_2_0A05
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
loc_2_09E4:
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        db  070h, 072h
loc_2_0A05:
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
loc_2_0A0B:
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        jo     SHORT loc_2_0A89
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0A99
loc_2_0A34:
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        popad
        jb     NEAR PTR loc_2_0AB5
        db  073h, 073h
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        jo     SHORT loc_2_0AD0
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
loc_2_0A89:
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        jb     SHORT loc_2_0AB2
        arpl   word ptr [ecx + 6ch], sp
        insb
        bound  esp, qword ptr [ecx + 63h]
loc_2_0A99:
        imul   esp, dword ptr [eax], 63h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_2_0B17
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edi + 70h], ch
        db  074h, 069h
        outsd
        outsb
loc_2_0AB2:
        or     al, byte ptr cs:[eax]
loc_2_0AB5:
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
loc_2_0AD0:
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        insd
        popad
        imul   esp, dword ptr [bp + di + 20h], 626d756eh
        db  065h, 072h, 020h
        outsd
        jo     SHORT loc_2_0B5D
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [edx + 65h], dh
        push   65h
        arpl   word ptr [eax + 66h], si
        outsd
        db  072h, 020h
        inc    ecx
        inc    ebx
        inc    ebx
        dec    ebp
loc_2_0B17:
        and    byte ptr [edi + 70h], ch
        je     SHORT loc_2_0B85
        outsd
        outsb
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0B93
        and    byte ptr fs:[ebx + 6fh], ah
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_2_0B9D
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6572h
        and    byte ptr [esi + 61h], ch
        imul   esp, dword ptr [eax], 66h
        outsd
        jb     SHORT loc_2_0B6C
        outsd
        db  070h, 074h
        imul   ebp, dword ptr [edi + 6eh], 2e642520h
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
loc_2_0B5D:
        and    byte ptr [eax + 65h], dl
        jb     NEAR PTR loc_2_0B83
        db  072h, 065h
        db  071h, 075h
        db  065h, 073h, 074h
        jae    SHORT loc_2_0B8C
loc_2_0B6C:
        dec    ebp
        push   edx
        push   ebp
        and    byte ptr [edi + 66h], ch
        and    byte ptr [0a2e64h], ah
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
loc_2_0B83:
        db  072h, 065h
loc_2_0B85:
        db  071h, 075h
        db  065h, 073h, 074h
        jae    SHORT loc_2_0BAC
loc_2_0B8C:
        inc    ecx
        inc    ebx
        inc    ebx
        dec    ebp
        and    byte ptr [edi + 66h], ch
loc_2_0B93:
        and    byte ptr [0a2e786ch], ah
        add    byte ptr [ebx + eax*2 + 50h], cl
loc_2_0B9D:
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        jb     NEAR PTR loc_2_0BC5
        jb     SHORT loc_2_0C0C
        jno    SHORT loc_2_0C1E
        jae    NEAR PTR loc_2_0C20
loc_2_0BAC:
        db  073h, 020h
        popad
        db  075h, 074h
        push   69746e65h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
loc_2_0BC5:
        and    byte ptr [25282078h], ah
        jae    SHORT loc_2_0BF6
        or     al, byte ptr cs:[eax]
        dec    esp
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        jb     NEAR PTR loc_2_0BFB
        db  072h, 065h
        db  071h, 075h
        db  065h, 073h, 074h
        jae    SHORT loc_2_0C04
        insb
        imul   ebp, dword ptr [esi + 6bh], 61757120h
        insb
        imul   esi, dword ptr [ecx + edi*2 + 20h], 746f7270h
        outsd
loc_2_0BF6:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [eax], ch
loc_2_0BFB:
        jne    SHORT loc_2_0C6B
        jae    SHORT loc_2_0C74
        db  070h, 070h
        outsd
        db  072h, 074h
loc_2_0C04:
        sub    dword ptr fs:[esi], ebp
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
loc_2_0C0C:
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        jb     SHORT loc_2_0C7C
        jno    SHORT loc_2_0C8E
        db  065h, 073h, 074h
        db  073h, 020h
loc_2_0C1E:
        db  075h, 073h
loc_2_0C20:
        and    byte ptr gs:[edi + 66h], ch
        and    byte ptr [ebp + 61h], ch
        imul   esp, dword ptr [bp + di + 20h], 626d756eh
        jb     NEAR PTR loc_2_0C52
        and    eax, 0a2e786ch
        add    byte ptr [ebx + eax*2 + 50h], cl
        and    byte ptr [edx], bh
        and    byte ptr [edi + ebp*2 + 6fh], cl
        db  070h, 062h
        popad
        arpl   word ptr [ebx + 21h], bp
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        outsb
        popad
        imul   esp, dword ptr [edi], 64h
loc_2_0C52:
        and    byte ptr [edi + 69h], dh
        db  074h, 068h
        and    byte ptr [edi + 75h], ch
        jb     SHORT loc_2_0C7C
        outsb
        popad
        imul   esp, dword ptr [eax], 6fh
        and    byte ptr [eax + 65h], dh
        jb     NEAR PTR loc_2_0C8F
        db  073h, 020h
        insd
loc_2_0C6B:
        popad
        imul   esp, dword ptr [bp + di + 20h], 626d756eh
loc_2_0C74:
        jb     NEAR PTR loc_2_0C98
        or     al, byte ptr [eax]
        dec    esp
        inc    ebx
        push   eax
loc_2_0C7C:
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        jb     NEAR PTR loc_2_0CA4
        db  072h, 065h
        jne    NEAR PTR loc_2_0CFC
        and    byte ptr fs:[eax + 72h], dh
loc_2_0C8E:
        outsd
loc_2_0C8F:
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [ebx + 6fh], ah
        insd
loc_2_0C98:
        db  070h, 072h
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
loc_2_0CA4:
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        jb     NEAR PTR loc_2_0CD5
        db  072h, 065h
        db  071h, 075h
        db  065h, 073h, 074h
        db  073h, 020h
        db  070h, 072h
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        db  070h, 072h
        db  065h, 073h, 073h
loc_2_0CD5:
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        jb     SHORT loc_2_0D54
        jne    NEAR PTR loc_2_0D65
        and    byte ptr fs:[ecx + 64h], ah
        jb     NEAR PTR loc_2_0D5F
        db  073h, 073h
loc_2_0CFC:
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        jo     SHORT loc_2_0D7A
        jae    NEAR PTR loc_2_0D7E
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        db  072h, 065h
        jno    SHORT loc_2_0D9C
        jae    NEAR PTR loc_2_0D9E
        db  073h, 020h
        popad
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        jo     SHORT loc_2_0DB1
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74706f20h
        imul   ebp, dword ptr [edi + 6eh], 4c000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
loc_2_0D54:
        and    byte ptr [eax + 65h], dl
        jb     NEAR PTR loc_2_0D7A
        jb     SHORT loc_2_0DC1
        db  066h, 075h, 073h
loc_2_0D5F:
        and    byte ptr fs:[ebx + 61h], ah
        insb
loc_2_0D65:
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 70h
        db  072h, 06Fh
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edi + 70h], ch
        je     SHORT loc_2_0DE1
        outsd
        outsb
loc_2_0D7A:
        or     al, byte ptr cs:[eax]
        dec    esp
loc_2_0D7E:
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + esi*2 + 79h], dl
        imul   ebp, dword ptr [esi + 67h], 74756120h
        push   69746e65h
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
loc_2_0D9C:
        je     SHORT loc_2_0E0D
loc_2_0D9E:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [25282078h], ah
        db  073h, 029h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
loc_2_0DB1:
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 68h], al
        popad
        outsb
        and    byte ptr fs:[bp + 65h], ch
        db  074h, 020h
        outsb
loc_2_0DC1:
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [6f742078h], ah
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
loc_2_0DE1:
        dec    ebx
        and    byte ptr [esi + 65h], ch
        db  074h, 020h
        outsb
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [69772078h], ah
        db  074h, 068h
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 63h], al
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_0E72
loc_2_0E0D:
        and    byte ptr fs:[ebp + 61h], ch
        db  074h, 063h
        push   20676e69h
        outsb
        db  065h, 074h, 020h
        outsb
        jne    SHORT loc_2_0E8C
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[eax + 65h], dh
        jb     NEAR PTR loc_2_0E6B
        outsb
        outsd
        and    byte ptr gs:[esi + 75h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [78252078h], ah
        and    byte ptr [78252078h], ah
        and    byte ptr [78252078h], ah
        or     al, byte ptr cs:[eax]
loc_2_0E6B:
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
loc_2_0E72:
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_0EEC
        and    byte ptr fs:[ebx + 6fh], ah
        insd
loc_2_0E8C:
        db  070h, 072h
        jae    NEAR PTR loc_2_0F04
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        je     SHORT loc_2_0F09
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edx + 65h], dh
        jno    SHORT loc_2_0F17
        db  065h, 073h, 074h
        and    byte ptr [78252078h], ah
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0F22
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[edx + 6fh], dh
        db  075h, 074h
        imul   ebp, dword ptr [esi + 67h], 6f727020h
        je     SHORT loc_2_0F48
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
loc_2_0EEC:
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0F56
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_0F63
        and    byte ptr fs:[edx + 6fh], dh
        db  075h, 074h
loc_2_0F04:
        db  065h, 072h, 020h
        outsb
        popad
loc_2_0F09:
        insd
        and    byte ptr gs:[25632563h], ah
        arpl   word ptr [25632563h], sp
loc_2_0F17:
        arpl   word ptr [2e632563h], sp
        or     al, byte ptr [eax]
        dec    ecx
        push   eax
        pop    eax
loc_2_0F22:
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0F93
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_0FA0
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
loc_2_0F48:
        outsd
        outsb
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 06Ch
        je     NEAR PTR loc_2_0FB8
        or     al, byte ptr cs:[eax]
loc_2_0F56:
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
loc_2_0F63:
        db  076h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        db  070h, 072h
        jae    NEAR PTR loc_2_0FE2
        imul   ebp, dword ptr [edi + 6eh], 4b414e20h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        pop    eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0FED
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
loc_2_0F93:
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_1009
        db  065h, 074h, 065h
loc_2_0FA0:
        and    byte ptr [esi + 41h], cl
        dec    ebx
        or     al, byte ptr cs:[eax]
        inc    ebx
        dec    ecx
        push   eax
        pop    eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 65h], dl
        db  065h, 072h, 020h
        db  073h, 065h
        outsb
        db  074h, 020h
loc_2_0FB8:
        dec    esi
        inc    ebx
        push   eax
        and    byte ptr [ebx + 6fh], ah
        insd
        jo     SHORT loc_2_1033
        db  065h, 073h, 073h
        imul   esp, dword ptr [edx + 6ch], 65682065h
        popad
        db  064h, 065h, 072h, 020h
        outsd
        outsb
        and    byte ptr [ebx + 6fh], ah
        outsb
        imul   si, word ptr [edx + 6dh], 6465h
        and    byte ptr [ebx + 6ch], dh
        outsd
        db  074h, 021h
loc_2_0FE2:
        or     al, byte ptr [eax]
        inc    ebx
        dec    ecx
        push   eax
        pop    eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
loc_2_0FED:
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6eh], si
        and    byte ptr [di + 6eh], dh
        db  072h, 065h
        arpl   word ptr [edi + 67h], bp
        outsb
        imul   edi, dword ptr [edx + 65h], 61702064h
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
loc_2_1009:
        je     SHORT loc_2_1084
        jo     SHORT loc_2_1072
        and    byte ptr [edi + 72h], ch
        and    byte ptr [esi + 6ch], ah
        popad
        and    byte ptr [bx + si], ch
        and    eax, 29583230h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 6fh], al
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        arpl   word ptr [edi + 6dh], bp
loc_2_1033:
        insd
        db  075h, 06Eh
        imul   esp, dword ptr [ebx + 61h], 77206574h
        imul   esi, dword ptr [eax + ebp*2 + 20h], 2f504354h
        dec    ecx
        push   eax
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 070h
        outsd
        db  072h, 074h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 50h], cl
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        jae    SHORT loc_2_10D8
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
loc_2_1072:
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
loc_2_1084:
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ecx + 50h], cl
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        jae    SHORT loc_2_1108
        db  065h, 073h, 020h
        outsd
        and    byte ptr [6e612078h], ah
        and    byte ptr fs:[0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_111A
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        db  070h, 072h
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 6f727020h
        je     SHORT loc_2_1144
        arpl   word ptr [edi + 6ch], bp
loc_2_10D8:
        and    byte ptr [edx + 65h], dh
        db  071h, 075h
        jae    NEAR PTR loc_2_1154
        and    byte ptr [78252078h], ah
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_115C
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ecx + 50h], cl
loc_2_1108:
        and    byte ptr [ecx + 64h], ah
        jb     NEAR PTR loc_2_1173
        db  073h, 073h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [0a2e78h], ah
        dec    ecx
loc_2_111A:
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 68h], al
        popad
        outsb
        imul   ebp, dword ptr [bp + 67h], 20504920h
        popad
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_2_11A6
        and    byte ptr [esi + 72h], ah
        outsd
        insd
        and    byte ptr [6f742078h], ah
        and    byte ptr [0a2e78h], ah
loc_2_1144:
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
        dec    ebx
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
loc_2_1154:
        jbe    SHORT loc_2_11BB
        and    byte ptr fs:[esi + 6fh], ah
        db  072h, 020h
loc_2_115C:
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_2_11D3
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 49000a2eh
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 63h], al
loc_2_1173:
        arpl   word ptr [ebp + 70h], sp
        db  074h, 069h
        outsb
        and    byte ptr [bx + si + 72h], dh
        imul   ebp, dword ptr [ebp + 61h], 44207972h
        dec    esi
        push   ebx
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 63h], al
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_1203
        outsb
        and    byte ptr [bx + si + 72h], dh
        imul   ebp, dword ptr [ebp + 61h], 57207972h
loc_2_11A6:
        dec    ecx
        dec    esi
        push   ebx
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 63h], al
        arpl   word ptr [ebp + 70h], sp
loc_2_11BB:
        db  074h, 069h
        outsb
        and    byte ptr [bp + si + 61h], ah
        arpl   word ptr [ebx + 75h], bp
        db  070h, 020h
        inc    esp
        dec    esi
        push   ebx
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
loc_2_11D3:
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 63h], al
        arpl   word ptr [ebp + 70h], sp
        db  074h, 069h
        outsb
        and    byte ptr [bp + si + 61h], ah
        arpl   word ptr [ebx + 75h], bp
        db  070h, 020h
        push   edi
        dec    ecx
        dec    esi
        push   ebx
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 6fh], cl
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        jae    SHORT loc_2_1276
loc_2_1203:
        db  065h, 073h, 020h
        outsb
        outsd
        db  074h, 069h
        popad
        db  074h, 065h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        arpl   word ptr [ebp + 61h], bp
        jb     SHORT loc_2_124A
        dec    ecx
        push   eax
        and    byte ptr [ecx + 64h], ah
        jb     NEAR PTR loc_2_1297
        db  073h, 073h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        db  073h, 065h
loc_2_124A:
        db  074h, 020h
        dec    ecx
        push   eax
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 61h], al
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        db  073h, 065h
        je     SHORT loc_2_1291
        dec    ecx
        push   eax
        and    byte ptr [edx + 6fh], dh
loc_2_1276:
        db  075h, 074h
        and    byte ptr gs:[0a2e78h], ah
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ebp + esi*2 + 72h], dl
        outsb
        imul   ebp, dword ptr [esi + 67h], 66666f20h
loc_2_1291:
        and    byte ptr [ecx + 50h], cl
        and    byte ptr [eax + 65h], ch
loc_2_1297:
        popad
        db  064h, 065h, 072h, 020h
        arpl   word ptr [edi + 6dh], bp
        db  070h, 072h
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 49000a2eh
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ecx + 50h], cl
        and    byte ptr [eax + 65h], cl
        popad
        jb     NEAR PTR loc_2_12DB
        arpl   word ptr [edi + 6dh], bp
        db  070h, 072h
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 66666f20h
        and    byte ptr [edx + 6fh], dh
        insb
        insb
        imul   ebp, dword ptr [esi + 67h], 63616220h
        imul   esp, dword ptr [eax], 73h
        je     SHORT loc_2_133C
loc_2_12DB:
        db  074h, 065h
        or     al, byte ptr cs:[eax]
        dec    ecx
        push   eax
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ebp + 6eh], dl
        imul   esp, dword ptr [ecx + ebp*2 + 72h], 69746365h
        outsd
        outsb
        popad
        insb
        and    byte ptr [ecx + 50h], cl
        and    byte ptr [eax + 65h], ch
        popad
        db  064h, 065h, 072h, 020h
        arpl   word ptr [edi + 6dh], bp
        db  070h, 072h
        jae    NEAR PTR loc_2_137B
        imul   ebp, dword ptr [edi + 6eh], 6f63202ch
        outsb
        jae    SHORT loc_2_137B
        db  064h, 065h, 072h, 020h
        db  074h, 075h
        db  072h, 06Eh
        imul   ebp, dword ptr [esi + 67h], 20746920h
        outsd
        and    byte ptr [0a2e78h], ah
        dec    esi
        inc    edx
        inc    esi
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [eax + 72h], dl
        outsd
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
loc_2_133C:
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 4e000a2eh
        inc    edx
        inc    esi
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 75h], dl
        db  073h, 070h
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 726f6620h
        and    byte ptr [eax + 72h], dh
        outsd
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        or     al, byte ptr cs:[eax]
        dec    esi
        inc    edx
        inc    esi
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
        dec    ebx
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
loc_2_137B:
        db  076h, 065h
        and    byte ptr fs:[6f725020h], ch
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [esi + 61h], ch
        insd
        and    byte ptr gs:[ebp + 69h], ch
        db  073h, 06Dh
        popad
        je     SHORT loc_2_13FB
        push   4e000a2eh
        inc    edx
        inc    esi
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
        dec    ebx
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[6f725020h], ch
        push   65h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 43000a2eh
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_1439
        and    byte ptr fs:[ebp + 72h], dh
        insd
        imul   ebp, dword ptr [esi + 61h], 72206574h
        db  065h, 071h, 075h
        db  065h, 073h, 074h
        or     al, byte ptr cs:[eax]
        inc    ebx
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [esi + 41h], cl
        dec    ebx
        and    byte ptr [ebp + 6eh], dh
        db  072h, 065h
        arpl   word ptr [edi + 67h], bp
loc_2_13FB:
        outsb
        imul   edi, dword ptr [edx + 65h], 6f632064h
        insd
        jo     SHORT loc_2_1478
        jae    NEAR PTR loc_2_147C
        imul   ebp, dword ptr [edi + 6eh], 6c617620h
        db  075h, 065h
        and    byte ptr [0a2e78h], ah
        inc    ebx
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[ebx + 6fh], ah
        insd
        db  070h, 072h
loc_2_1439:
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 6c617620h
        db  075h, 065h
        and    byte ptr [0a2e78h], ah
        inc    ebx
        inc    ebx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [ebx + 65h], dl
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 6d6f6320h
        db  070h, 072h
        jae    NEAR PTR loc_2_14D4
        imul   ebp, dword ptr [edi + 6eh], 73657220h
        je     NEAR PTR loc_2_148B
        and    eax, 0a2e64h
        push   eax
        inc    ecx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
loc_2_1478:
        insd
        outsd
        db  074h, 065h
loc_2_147C:
        and    byte ptr [eax + 50h], dl
        push   eax
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edx + 72h], ah
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
loc_2_148B:
        popad
        outsb
        and    byte ptr fs:[ebx + 68h], dh
        outsd
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        db  073h, 065h
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 63617020h
        imul   esp, dword ptr [ebp + 74h], 20h
        db  074h, 079h
        db  070h, 065h
        and    byte ptr [0a2178h], ah
        push   eax
        inc    ecx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        insd
        outsd
        db  074h, 065h
        and    byte ptr [eax + 50h], dl
        push   eax
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edx + 72h], ah
        outsd
        imul   esp, dword ptr [ebp + 6eh], 2eh
        and    byte ptr [ebx + 65h], dl
loc_2_14D4:
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 74756120h
        push   6b63612dh
        and    byte ptr [edi + ebp*2 + 20h], dh
        je     SHORT loc_2_155A
        db  079h, 020h
        popad
        outsb
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        je     SHORT loc_2_155C
        outsb
        db  075h, 065h
        or     al, byte ptr cs:[eax]
        push   eax
        inc    ecx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edi + ebp*2 + 67h], cl
        imul   ebp, dword ptr [esi + 20h], 20736177h
        jae    SHORT loc_2_1580
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        jne    NEAR PTR loc_2_157F
        or     al, byte ptr cs:[eax]
        push   eax
        inc    ecx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edi + ebp*2 + 67h], cl
        imul   ebp, dword ptr [esi + 20h], 6c696166h
        cmp    ah, byte ptr fs:[eax]
        db  075h, 073h
        db  065h, 072h, 06Eh
        popad
        insd
        and    byte ptr gs:[edi + 72h], ch
        and    byte ptr [eax + 61h], dh
        db  073h, 073h
        db  077h, 06Fh
        db  072h, 064h
        and    byte ptr [edi + 61h], dh
        jae    SHORT loc_2_1563
        imul   ebp, dword ptr [esi + 63h], 6572726fh
        arpl   word ptr [esi + ebp + 0ah], si
        add    byte ptr [ebx + 48h], al
        inc    ecx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edx + 65h], dl
        insd
        outsd
loc_2_155A:
        db  074h, 065h
loc_2_155C:
        and    byte ptr [eax + 50h], dl
        push   eax
        and    byte ptr [ecx + 73h], ch
loc_2_1563:
        and    byte ptr [edx + 72h], ah
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        popad
        outsb
        and    byte ptr fs:[ebx + 68h], dh
        outsd
        jne    SHORT loc_2_15E0
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_159A
        bound  esp, qword ptr [ebp + 20h]
        jae    SHORT loc_2_15E4
loc_2_157F:
        outsb
loc_2_1580:
        imul   ebp, dword ptr fs:[esi + 67h], 63617020h
        imul   esp, dword ptr [ebp + 74h], 20h
        db  074h, 079h
        db  070h, 065h
        and    byte ptr [0a2178h], ah
        inc    ebx
        dec    eax
        inc    ecx
        push   eax
loc_2_159A:
        and    byte ptr [edx], bh
        and    byte ptr [edi + ebp*2 + 67h], cl
        imul   ebp, dword ptr [esi + 20h], 20736177h
        jae    SHORT loc_2_161E
        arpl   word ptr [ebx + 65h], sp
        jae    SHORT loc_2_1621
        jne    NEAR PTR loc_2_161D
        or     al, byte ptr cs:[eax]
        inc    ebx
        dec    eax
        inc    ecx
        push   eax
        and    byte ptr [edx], bh
        and    byte ptr [edi + ebp*2 + 67h], cl
        imul   ebp, dword ptr [esi + 20h], 6c696166h
        cmp    ah, byte ptr fs:[eax]
        jne    SHORT loc_2_163E
        db  065h, 072h, 06Eh
        popad
        insd
        sub    al, 20h
        db  070h, 061h
        db  073h, 073h
        db  077h, 06Fh
        jb     SHORT loc_2_163F
        sub    al, 20h
        outsd
        db  072h, 020h
loc_2_15E0:
        outsd
        insd
        popad
loc_2_15E4:
        imul   ebp, dword ptr [esi + 20h], 20736177h
        imul   ebp, dword ptr [esi + 63h], 6572726fh
        arpl   word ptr [esi + ebp + 0ah], si
        add    byte ptr [ebx + 61h], al
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 3ah
        and    byte ptr [ebx + 6bh], dl
        imul   esi, dword ptr [eax + 70h], 20676e69h
        arpl   word ptr [ecx + 6ch], sp
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 61h
        outsb
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        db  074h, 069h
loc_2_161D:
        outsb
loc_2_161E:
        db  075h, 069h
        outsb
loc_2_1621:
        and    byte ptr [bx + 69h], dh
        db  074h, 068h
        and    byte ptr [ebx + 75h], ah
        jb     SHORT loc_2_169E
        outsb
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        or     al, byte ptr cs:[eax]
        inc    ebx
loc_2_163E:
        popad
loc_2_163F:
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 3ah
        and    byte ptr [ebp + 6ch], dl
        insb
        imul   ebp, dword ptr [esi + 67h], 72657320h
        db  076h, 065h
        db  072h, 020h
        je     SHORT loc_2_16C8
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 61h
        db  074h, 020h
        jne    SHORT loc_2_16DB
        db  065h, 072h, 020h
        jae    SHORT loc_2_16DD
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 6d756e20h
        bound  esp, qword ptr [ebp + 72h]
        or     al, byte ptr cs:[eax]
        inc    ebx
        popad
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 3ah
        and    byte ptr [ebp + 6ch], dl
        insb
        imul   ebp, dword ptr [esi + 67h], 72657320h
        jbe    SHORT loc_2_16FB
        db  072h, 020h
        db  074h, 06Fh
        and    byte ptr [ebx + 61h], ah
        insb
loc_2_169E:
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [eax], 61h
        db  074h, 020h
        popad
        insd
        imul   ebp, dword ptr [esi + 69h], 61727473h
        je     SHORT loc_2_1722
        db  072h, 020h
        jae    SHORT loc_2_1727
        arpl   word ptr gs:[ecx + 66h], bp
        imul   esp, dword ptr [ebp + 64h], 6d756e20h
        bound  esp, qword ptr [ebp + 72h]
        or     al, byte ptr cs:[eax]
loc_2_16C8:
        and    eax, 203a2073h
        dec    esp
        popad
        db  079h, 065h
        db  072h, 020h
        and    eax, 0a2e73h
        push   eax
        push   eax
        push   eax
loc_2_16DB:
        and    byte ptr [eax], ch
loc_2_16DD:
        push   eax
        outsd
        imul   ebp, dword ptr [esi + 74h], 206f7420h
        push   eax
        outsd
        imul   ebp, dword ptr [esi + 74h], 6f725020h
        je     SHORT loc_2_1760
        arpl   word ptr [edi + 6ch], bp
        sub    dword ptr [eax], eax
        push   edx
        inc    ecx
        push   ebx
        and    byte ptr [eax], ch
loc_2_16FB:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 2073776fh
        dec    esi
        push   esp
        and    byte ptr [ebx], dh
        xor    dword ptr cs:[eax], esp
        push   edx
        insd
        outsd
        db  074h, 065h
        and    byte ptr [ecx + 63h], al
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        push   ebx
        jb     NEAR PTR loc_2_1792
        jb     NEAR PTR loc_2_1748
        add    byte ptr [ebx + 4ch], dl
loc_2_1722:
        dec    ecx
        push   eax
        and    byte ptr [eax], ch
        push   ebx
loc_2_1727:
        jb     NEAR PTR loc_2_1793
        popad
        insb
        and    byte ptr [ecx + ebp*2 + 6eh], cl
        and    byte ptr gs:[eax + 72h], dl
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        sub    dword ptr [eax], eax
        inc    ebx
        push   ebx
        dec    esp
        dec    ecx
        push   eax
        and    byte ptr [eax], ch
        push   ebx
        jb     NEAR PTR loc_2_17B0
        popad
loc_2_1748:
        insb
        and    byte ptr [ecx + ebp*2 + 6eh], cl
        and    byte ptr gs:[eax + 72h], dl
        outsd
        db  074h, 06Fh
        arpl   word ptr [edi + 6ch], bp
        sub    dword ptr [eax], esp
        sub    esp, dword ptr [eax]
        dec    ecx
        push   eax
        and    byte ptr [eax + 65h], cl
loc_2_1760:
        popad
        db  064h, 065h, 072h, 020h
        inc    ebx
        outsd
        insd
        db  070h, 072h
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 43574e00h
        and    byte ptr [eax], ch
        dec    esi
        db  065h, 074h, 077h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 6fh], al
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp], si
        dec    esp
        inc    ebx
        push   eax
        add    byte ptr [ecx + 50h], cl
        inc    ebx
        push   eax
        add    byte ptr [esi + 42h], cl
loc_2_1792:
        inc    esi
loc_2_1793:
        inc    ebx
        push   eax
        add    byte ptr [ecx + 50h], cl
        pop    eax
        inc    ebx
        push   eax
        add    byte ptr [ebx + 43h], al
        push   eax
        add    byte ptr [ebx + 48h], al
        inc    ecx
        push   eax
        add    byte ptr [eax + 41h], dl
        push   eax
        add    byte ptr [ebx + 61h], al
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
loc_2_17B0:
        imul   eax, dword ptr [ebx + 50h], 0
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 70750064h
        add    byte ptr [edi + ebp*2 + 77h], ah
        outsb
        add    byte ptr [ebx + 74h], dh
        popad
        db  072h, 074h
        add    byte ptr fs:[esi + 69h], ah
        outsb
        imul   esi, dword ptr [ebx + 68h], 74006465h
        db  065h, 072h, 06Dh
        imul   ebp, dword ptr [esi + 61h], 646574h
        outsb
        outsd
        and    byte ptr [ebp + 73h], ah
        arpl   word ptr [edx + 69h], si
        jo     SHORT loc_2_1863
        imul   ebp, dword ptr [edi + 6eh], 43524300h
        and    byte ptr [ebp + 72h], al
        jb     SHORT loc_2_186A
        db  072h, 073h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [54000a64h], ah
        imul   ebp, dword ptr [ebp + 65h], 2074756fh
        inc    ebp
        jb     SHORT loc_2_188B
        outsd
        jb     SHORT loc_2_188F
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [41000a64h], ah
        insb
        imul   esp, dword ptr [edi + 6eh], 746e656dh
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        db  072h, 073h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [4f000a64h], ah
        db  076h, 065h
        jb     SHORT loc_2_18BB
        db  075h, 06Eh
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        db  072h, 073h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [46000a64h], ah
        db  072h, 061h
        insd
loc_2_1863:
        imul   ebp, dword ptr [esi + 67h], 72724520h
loc_2_186A:
        outsd
        db  072h, 073h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [42000a64h], ah
        db  075h, 066h
        db  066h, 065h, 072h, 020h
        dec    edi
        jbe    SHORT loc_2_18E9
        db  072h, 072h
        db  075h, 06Eh
        and    byte ptr [ebp + 72h], al
loc_2_188B:
        db  072h, 06Fh
        jb     SHORT loc_2_1902
loc_2_188F:
        and    byte ptr [eax], ah
        and    eax, 49000a64h
        outsb
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_2_1908
        je     NEAR PTR loc_2_1904
        and    byte ptr [eax + 61h], dl
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [42000a64h], ah
        db  079h, 074h
        db  065h, 073h, 020h
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
loc_2_18BB:
        jbe    SHORT loc_2_1922
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [42000a64h], ah
        jns    SHORT loc_2_1942
        db  065h, 073h, 020h
        push   esp
        db  072h, 061h
        outsb
        jae    SHORT loc_2_1944
        imul   esi, dword ptr [esp + esi*2 + 74h], 20206465h
        and    byte ptr [eax], ah
        and    byte ptr [46000a64h], ah
        db  072h, 061h
loc_2_18E9:
        insd
        db  065h, 073h, 020h
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    eax, 46000a64h
loc_2_1902:
        db  072h, 061h
loc_2_1904:
        insd
        jae    NEAR PTR loc_2_1928
loc_2_1908:
        push   esp
        db  072h, 061h
        outsb
        db  073h, 06Dh
        imul   esi, dword ptr [esp + esi*2 + 65h], 20202064h
        and    byte ptr [eax], ah
        and    eax, 49000a64h
        outsb
        db  073h, 074h
        popad
        insb
loc_2_1922:
        insb
        popad
        bound  ebp, qword ptr [ebp + 20h]
loc_2_1928:
        inc    ebx
        push   eax
        and    byte ptr [esi + 78h], dl
        inc    esp
        and    byte ptr [73692073h], ah
        and    byte ptr [edi + ebp*2 + 61h], ch
        or     al, byte ptr fs:[eax]
        dec    esi
        outsd
        and    byte ptr [ecx + 6eh], cl
loc_2_1942:
        db  073h, 074h
loc_2_1944:
        popad
        insb
        insb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        inc    ebx
        push   eax
        and    byte ptr [esi + 78h], dl
        inc    esp
        daa
        db  073h, 020h
        popad
        db  072h, 065h
        and    byte ptr [edi + ebp*2 + 61h], ch
        or     al, byte ptr fs:[eax]
_PTEXT ends
        end
