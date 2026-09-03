; dlc_pcode_02.asm -- DLC object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  03Ah, 000h, 000h, 000h
loc_2_0004:
        test   byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
loc_2_000A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    dword ptr [eax], eax
        inc    eax
        add    byte ptr [ebx], bl
        ja     SHORT loc_2_0016
loc_2_0016:
        xchg   edx, eax
        add    byte ptr [eax + eax + 0e600c5h], ah
        cli
        add    byte ptr [esi], dl
        add    dword ptr [5e014001h], ebp
        add    dword ptr [esi + 1], edi
        test   eax, 0c801b701h
        add    edx, esp
        add    ebx, edi
        add    dword ptr [edx], esp
        add    al, byte ptr [edx + 2]
        arpl   word ptr [edx], ax
        db  07Dh, 002h
        pushfd
        add    dh, dh
        add    dl, byte ptr [edx]
        add    esi, dword ptr [83035a03h]
        add    esp, dword ptr [edx - 1bfc41fdh]
        add    edi, esi
        add    ebx, dword ptr [edi]
        add    al, 46h
        add    al, 5ah
        add    al, 72h
        add    al, 87h
        add    al, 0a5h
        add    al, 0bch
        add    al, 0d9h
        add    al, 0f0h
        add    al, 8
        add    eax, 5310523h
        push   ebp
        add    eax, 59c0579h
        mov    ecx, 5e005h
        push   es
        and    dword ptr [esi], eax
        push   esp
        push   es
        jo     SHORT loc_2_0080
        mov    word ptr [esi], es
        lodsb
        push   es
        ret
        db  006h
loc_2_0080:
        fadd   qword ptr [esi]
        add    byte ptr [eax], al
loc_2_0084:
        inc    edx
        popad
        and    byte ptr fs:[ebp + 76h], ah
        outsb
        db  074h, 020h
        je     SHORT loc_2_0109
        db  070h, 065h
        and    byte ptr [esi + 6fh], ah
        jb     SHORT loc_2_00B7
        jae    SHORT loc_2_0112
        db  073h, 074h
        insd
        and    byte ptr [ebp + 76h], ah
        outsb
        je     SHORT loc_2_00C5
        add    byte ptr [edx + 61h], al
        and    byte ptr fs:[edx + esi*2 + 61h], dl
        outsb
        jae    SHORT loc_2_011C
        imul   esi, dword ptr [eax + 50h], 656b6361h
loc_2_00B7:
        je     SHORT loc_2_00D9
        inc    esp
        jae    NEAR PTR loc_2_0120
        db  072h, 069h
        db  070h, 074h
        outsd
        jb     SHORT loc_2_00C4
loc_2_00C4:
        inc    ebx
loc_2_00C5:
        popad
        outsb
        daa
        db  074h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[ebp + 73h], dh
        db  065h, 072h, 020h
        insd
loc_2_00D9:
        insd
        outsd
        jb     SHORT loc_2_0157
        add    byte ptr [ebx + 61h], al
        outsb
        daa
        db  074h, 020h
        imul   bp, word ptr [esi + 64h], 6720h
        db  072h, 06Fh
        jne    SHORT loc_2_015F
        and    byte ptr [ebp + 65h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [ebx + 41h], dl
        push   eax
        add    byte ptr [ebx + 61h], al
        outsb
        daa
        je     SHORT loc_2_0121
        db  066h, 072h, 065h
        and    byte ptr gs:[esp + ecx*2 + 43h], al
loc_2_0109:
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        push   eax
loc_2_0112:
        outsd
        outsd
        insb
        add    byte ptr [ebx + 61h], al
        outsb
        daa
        db  074h, 020h
loc_2_011C:
        db  067h, 065h, 074h, 020h
loc_2_0120:
        inc    ecx
loc_2_0121:
        push   ebp
        pop    eax
        pop    edi
        inc    ebx
        inc    ebx
        inc    edx
        add    byte ptr [ebx + 61h], al
        outsb
        daa
        db  074h, 020h
        db  067h, 065h, 074h, 020h
        imul   ebp, dword ptr [esi + 73h], 636e6174h
        and    byte ptr gs:[ebx + 6fh], ah
        outsb
        imul   sp, word ptr [edi + 75h], 6172h
        db  074h, 069h
        outsd
        outsb
        add    byte ptr [ebx + 61h], al
        outsb
        daa
        db  074h, 020h
        je     NEAR PTR loc_2_0173
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
loc_2_0157:
        and    byte ptr [edx + 75h], ah
        jb     NEAR PTR loc_2_017F
loc_2_015F:
        jae    NEAR PTR loc_2_01C6
        jb     SHORT loc_2_01CE
        jo     SHORT loc_2_01DB
        outsd
        jb     SHORT loc_2_016A
loc_2_016A:
        inc    ebx
        popad
        outsb
        daa
        je     SHORT loc_2_0190
        outsd
        jo     SHORT loc_2_01D8
loc_2_0173:
        outsb
        and    byte ptr [edx + 65h], dl
        imul   esi, dword ptr [bp + di + 74h], 43007972h
loc_2_017F:
        popad
        outsb
        daa
        je     SHORT loc_2_01A4
        jb     SHORT loc_2_01EB
        imul   esi, dword ptr [bp + di + 74h], 44207265h
        dec    esp
        inc    ebx
loc_2_0190:
        and    byte ptr [eax + 72h], dh
        outsd
        je     SHORT loc_2_0205
        arpl   word ptr [edi + 6ch], bp
        add    byte ptr [ebx + 61h], al
        outsb
        daa
        je     SHORT loc_2_01C0
        db  073h, 065h
        je     SHORT loc_2_01C4
loc_2_01A4:
        insb
        outsd
        bound  esp, qword ptr [ecx + 6ch]
        and    byte ptr [ecx + ebp*2 + 6dh], dh
        jb     NEAR PTR loc_2_01B1
loc_2_01B1:
        inc    ebx
        popad
        outsb
        daa
        db  074h, 020h
        jae    SHORT loc_2_0222
        outsb
        popad
        insb
        and    byte ptr [ebp + 76h], ah
loc_2_01C0:
        outsb
        je     SHORT loc_2_01C4
loc_2_01C4:
        inc    ebx
        popad
loc_2_01C6:
        outsb
        daa
        je     SHORT loc_2_01EA
        db  075h, 06Eh
        insb
        outsd
loc_2_01CE:
        arpl   word ptr [ebx + 20h], bp
        bound  esi, qword ptr [ebp + 66h]
        db  066h, 065h, 072h, 020h
loc_2_01D8:
        jo     SHORT loc_2_0249
        outsd
loc_2_01DB:
        insb
        and    byte ptr [eax + 61h], dh
        add    byte ptr gs:[bp + di + 61h], al
        outsb
        daa
        je     SHORT loc_2_0208
        jne    SHORT loc_2_0258
loc_2_01EA:
        insb
loc_2_01EB:
        outsd
        arpl   word ptr [ebx + 20h], bp
        inc    esp
        dec    esp
        inc    ebx
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        db  073h, 065h
        insd
        outsb
        je     SHORT loc_2_0202
loc_2_0202:
        inc    ebx
        popad
        outsb
loc_2_0205:
        arpl   word ptr [ebp + 6ch], sp
loc_2_0208:
        and    byte ptr [edx + 45h], dl
        inc    ebx
        inc    ebp
        dec    ecx
        push   esi
        inc    ebp
        and    byte ptr [ebp + 76h], ah
        outsb
        je     SHORT loc_2_0237
        db  077h, 069h
        je     SHORT loc_2_0283
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [ecx + 43h], al
        inc    edx
loc_2_0222:
        and    byte ptr [edx + 65h], dh
        db  073h, 06Fh
        jne    SHORT loc_2_029B
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [ebx + 43h], al
        inc    edx
        and    byte ptr [ebx + 6fh], ah
        jb     SHORT loc_2_02A7
        jne    SHORT loc_2_02A7
loc_2_0237:
        je     SHORT loc_2_029E
        add    byte ptr fs:[ebx + 43h], al
        inc    edx
        and    byte ptr [edi + 6fh], dh
        db  072h, 06Bh
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
loc_2_0249:
        imul   esp, dword ptr [eax + eax + 43h], 6e61656ch
        imul   ebp, dword ptr [esi + 67h], 20707520h
loc_2_0258:
        popad
        and    byte ptr [ebx + 6ch], ah
        outsd
        jae    SHORT loc_2_02C4
        and    byte ptr fs:[ecx + ebp*2 + 6eh], cl
        imul   eax, dword ptr [eax], 43h
        insb
        popad
        outsb
        imul   ebp, dword ptr [esi + 67h], 20707520h
        popad
        and    byte ptr [ebx + 6ch], ah
        outsd
        db  073h, 065h
        and    byte ptr fs:[ebx + 41h], dl
        push   eax
        add    byte ptr [ebx + 6dh], al
        inc    esp
loc_2_0283:
        outsd
        outsb
        sub    byte ptr gs:[ecx], ch
        and    byte ptr [ebp + 6eh], dl
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
        and    byte ptr [ebx + 43h], al
        inc    edx
        and    byte ptr [ecx + edi*2 + 70h], dh
        and    dword ptr gs:[eax], esp
loc_2_029B:
        sub    byte ptr [ebx + 43h], al
loc_2_029E:
        inc    edx
        pop    edi
        db  077h, 06Fh
        jb     SHORT loc_2_030F
        sub    dword ptr [eax], eax
        inc    ebx
loc_2_02A7:
        outsd
        jo     SHORT loc_2_0323
        and    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_2_02D2
        db  073h, 065h
        je     SHORT loc_2_02D6
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [ebp + 6eh], ch
        db  067h, 074h, 068h
loc_2_02C4:
        jae    SHORT loc_2_02C6
loc_2_02C6:
        inc    ebx
        outsd
        jo     SHORT loc_2_0343
        and    byte ptr [ebx + 4dh], dl
        inc    ecx
        dec    esp
        dec    esp
        pop    edi
        inc    esi
loc_2_02D2:
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
loc_2_02D6:
        and    byte ptr [ecx + 6eh], ch
        jbe    SHORT loc_2_033C
        insb
        imul   esp, dword ptr [eax + 6ch], 74676e65h
        push   4c440073h
        inc    ebx
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        push   eax
        outsd
        outsd
        insb
        and    byte ptr [ebx + 6fh], ah
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_2_0364
        add    byte ptr fs:[esp + ecx*2 + 43h], al
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        push   eax
        outsd
        outsd
loc_2_030F:
        insb
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 63h], 746e756fh
        jae    SHORT loc_2_0320
loc_2_0320:
        inc    esp
        dec    esp
        inc    ebx
loc_2_0323:
        dec    eax
        dec    esp
        push   eax
        inc    ebp
        pop    eax
        inc    ebp
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edx + 65h], dh
        jno    SHORT loc_2_03A7
        imul   esi, dword ptr [edx + 65h], 6f742064h
        and    byte ptr [ebx + 75h], dh
loc_2_033C:
        db  070h, 070h
        outsd
        jb     SHORT loc_2_03B5
        and    byte ptr [ecx], dh
loc_2_0343:
        and    byte ptr ss:[edx + 69h], ah
        db  074h, 020h
        popad
        jo     SHORT loc_2_03BC
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        db  073h, 00Ah
        dec    ecx
        je     SHORT loc_2_0379
        insd
        db  075h, 073h
        je     SHORT loc_2_037E
        bound  esp, qword ptr [ebp + 20h]
        insb
        outsd
        popad
loc_2_0364:
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [ecx + 55h], al
        push   esp
        dec    edi
        inc    ebp
        pop    eax
        inc    ebp
        inc    ebx
        inc    edx
        inc    ecx
        push   esp
loc_2_0379:
        add    byte ptr [esp + ebp*2 + 63h], al
        push   edx
loc_2_037E:
        arpl   word ptr gs:[ebp + 69h], sp
        jbe    SHORT loc_2_03E9
        sub    byte ptr [ecx], ch
        and    byte ptr [edx + 61h], ah
        and    byte ptr fs:[eax + 61h], dh
        db  072h, 061h
        insd
        je     NEAR PTR loc_2_03F8
        db  072h, 073h
        add    byte ptr [esp + ebp*2 + 63h], al
        push   edx
        arpl   word ptr gs:[ebp + 69h], sp
        db  076h, 065h
        sub    byte ptr [ecx], ch
        and    byte ptr [ecx + 6eh], ch
        db  076h, 061h
loc_2_03A7:
        insb
        imul   esp, dword ptr [eax + 68h], 65646165h
        db  072h, 020h
        bound  esi, qword ptr [ebp + 66h]
loc_2_03B5:
        jb     NEAR PTR loc_2_03B9
loc_2_03B9:
        inc    esp
        jne    SHORT loc_2_042C
loc_2_03BC:
        insb
        imul   esp, dword ptr [ebx + 61h], 44206574h
        dec    esp
        inc    ebx
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        and    byte ptr [ebp + 6eh], ch
        je     NEAR PTR loc_2_0444
        jae    SHORT loc_2_03DE
loc_2_03DE:
        inc    esi
        insb
        outsd
        ja     SHORT loc_2_0403
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_2_045A
        outsd
loc_2_03E9:
        insb
        and    byte ptr [ebp + 76h], ah
        outsb
        db  074h, 020h
        db  077h, 069h
        db  074h, 068h
        and    byte ptr [esi + 6fh], ch
loc_2_03F8:
        and    byte ptr [ecx + 43h], al
        inc    edx
        and    byte ptr [edx + 65h], dh
        db  073h, 06Fh
        jne    SHORT loc_2_0475
loc_2_0403:
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [edi + 72h], al
        outsd
        db  075h, 070h
        and    byte ptr [ebx + 41h], dl
        push   eax
        and    byte ptr [ebp + 65h], ch
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Eh
        db  074h, 020h
        imul   ebp, dword ptr [esi + 76h], 64696c61h
        add    byte ptr [ecx + 2dh], cl
        db  066h, 072h, 061h
        insd
loc_2_042C:
        and    byte ptr gs:[ecx + 63h], ah
        arpl   word ptr [ebp + 70h], sp
        db  074h, 065h
        and    byte ptr fs:[edx + 75h], ah
        db  074h, 020h
        outsb
        outsd
        and    byte ptr [ebx + 43h], al
        inc    edx
        add    byte ptr [ecx + 2dh], cl
loc_2_0444:
        jb     NEAR PTR loc_2_04A8
        insd
        and    byte ptr gs:[ecx + 6ch], ah
        db  072h, 065h
        popad
        db  064h, 079h, 020h
        popad
        arpl   word ptr [ebx + 65h], bp
        and    byte ptr fs:[ecx + 6eh], ah
loc_2_045A:
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0480
        db  070h, 065h
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 766e4900h
        popad
        insb
        imul   esp, dword ptr [eax + 44h], 4220434ch
loc_2_0475:
        db  075h, 066h
        db  066h, 065h, 072h, 020h
        push   65646165h
loc_2_0480:
        jb     SHORT loc_2_0482
loc_2_0482:
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 44h], 4220434ch
        db  075h, 066h
        db  066h, 065h, 072h, 020h
        push   eax
        outsd
        outsd
        insb
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   eax, dword ptr [eax], 49h
        outsb
        db  076h, 061h
        insb
loc_2_04A8:
        imul   esp, dword ptr [eax + 67h], 61626f6ch
        insb
        and    byte ptr [ecx + 64h], ah
        db  064h, 072h, 065h
        db  073h, 073h
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        bound  esi, qword ptr [ebp + 66h]
        db  066h, 065h, 072h, 020h
        jb     NEAR PTR loc_2_052D
        add    byte ptr gs:[ecx + 6eh], cl
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 4eh], 20534944h
        insd
        imul   esi, dword ptr fs:[ebp + 6dh], 766e4900h
        popad
        insb
        imul   esp, dword ptr [eax + 51h], 65756575h
        and    byte ptr [edx], dh
        and    byte ptr [edx + 75h], al
        jb     NEAR PTR loc_2_0568
        add    byte ptr [ecx + 6eh], cl
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 71h], 65756575h
        and    byte ptr fs:[ebp + 76h], ah
        outsb
        je     SHORT loc_2_050B
loc_2_050B:
        dec    ecx
        outsb
        db  076h, 061h
        insb
        imul   esp, dword ptr [eax + 72h], 69656365h
        db  076h, 065h
        and    byte ptr [ebp + 76h], ah
        outsb
        je     SHORT loc_2_0541
        outsw
        db  072h, 020h
        push   ebx
        inc    ecx
        push   eax
        add    byte ptr [ecx + 6eh], cl
        jbe    SHORT loc_2_058E
loc_2_052D:
        insb
        imul   esp, dword ptr [eax + 74h], 72656d69h
        and    byte ptr [ebx + 61h], ah
        insb
        insb
        bound  esp, qword ptr [ecx + 63h]
        imul   eax, dword ptr [eax], 4dh
loc_2_0541:
        inc    ecx
        inc    ebx
        and    byte ptr [eax + 65h], ch
        popad
        jb     NEAR PTR loc_2_056B
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        imul   ebp, dword ptr [edi + 6eh], 6e6f6320h
        jne    NEAR PTR loc_2_05CD
        add    byte ptr fs:[esi + 44h], cl
        dec    ecx
        push   ebx
        and    byte ptr [ebx + 61h], ah
        outsb
        daa
        je     SHORT loc_2_0588
loc_2_0568:
        db  073h, 065h
        outsb
loc_2_056B:
        and    byte ptr fs:[eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_2_0574
loc_2_0574:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        and    byte ptr [edi + 72h], dh
        popad
        jo     SHORT loc_2_05EE
        jb     NEAR PTR loc_2_05A1
        outsb
        outsd
        db  074h, 020h
        insb
        outsd
        popad
loc_2_0588:
        add    byte ptr fs:[esi + 6fh], cl
loc_2_058E:
        and    byte ptr [ecx + 2dh], cl
        jb     NEAR PTR loc_2_05F5
        insd
        and    byte ptr gs:[ebx + 43h], al
        inc    edx
        and    byte ptr [edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6dh], bp
loc_2_05A1:
        db  070h, 06Ch
        je     NEAR PTR loc_2_060B
        add    byte ptr [esi + 6fh], cl
        db  074h, 020h
        push   ebx
        db  079h, 073h
        je     SHORT loc_2_0615
        insd
        and    byte ptr [esi + 4dh], dl
        add    byte ptr [eax + 65h], dl
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 49444e20h
        push   ebx
        and    byte ptr [ebp + 76h], ah
        outsb
        je     SHORT loc_2_063B
        and    byte ptr [ecx + ebp*2 + 64h], ah
        outsb
loc_2_05CD:
        daa
        db  074h, 020h
        arpl   word ptr [edi + 6dh], bp
        db  070h, 06Ch
        db  065h, 074h, 065h
        add    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr [ebp + 76h], ah
        outsb
        db  074h, 020h
        db  077h, 069h
        db  074h, 068h
        and    byte ptr [esi + 6fh], ch
loc_2_05EE:
        and    byte ptr [ecx + 43h], al
        inc    edx
        and    byte ptr [edx + 65h], dh
loc_2_05F5:
        jae    SHORT loc_2_0666
        jne    SHORT loc_2_066B
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_0669
        and    byte ptr fs:[ecx + 2dh], cl
        db  066h, 072h, 061h
loc_2_060B:
        insd
        and    byte ptr gs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + ebp*2 + 6eh], cl
loc_2_0615:
        imul   esp, dword ptr [eax], 6eh
        outsd
        je     SHORT loc_2_063B
        outsd
        jo     SHORT loc_2_0683
        outsb
        add    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[ecx + 2dh], cl
        jb     NEAR PTR loc_2_068F
        insd
        and    byte ptr gs:[ecx + 6eh], ah
        and    byte ptr fs:[esi + 6fh], ch
        and    byte ptr [ecx + 4fh], al
        inc    ebx
loc_2_063B:
        inc    edx
        add    byte ptr [edx + 65h], dl
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr fs:[esp + ecx*2 + 43h], cl
        sub    eax, 72662032h
        popad
        insd
        and    byte ptr gs:[ecx + 6eh], ah
        and    byte ptr fs:[ecx + ebp*2 + 6eh], cl
        imul   esp, dword ptr [eax], 6eh
        outsd
        je     SHORT loc_2_067F
        outsd
        db  070h, 065h
        outsb
        add    byte ptr [edx + 65h], dl
loc_2_0666:
        arpl   word ptr [ebp + 69h], sp
loc_2_0669:
        jbe    SHORT loc_2_06D0
loc_2_066B:
        and    byte ptr fs:[ebx + 41h], dl
        inc    edx
        dec    ebp
        inc    ebp
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[ebx + 41h], dl
        push   eax
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_2_069F
loc_2_067F:
        outsd
        db  070h, 065h
        outsb
loc_2_0683:
        add    byte ptr [ebx + 65h], dl
        outsb
        and    byte ptr fs:[ebp + 76h], ah
        outsb
        db  074h, 020h
loc_2_068F:
        ja     SHORT loc_2_06FA
        je     SHORT loc_2_06FB
        and    byte ptr [esi + 6fh], ch
        and    byte ptr [ecx + 43h], al
        inc    edx
        and    byte ptr [edx + 65h], dh
        db  073h, 06Fh
loc_2_069F:
        jne    SHORT loc_2_0713
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        imul   esi, dword ptr [ebx + 20h], 61662061h
        db  074h, 061h
        insb
        and    byte ptr [ebp + 78h], ah
        imul   esi, dword ptr [eax + 74h], 20747365h
        insd
        db  065h, 073h, 073h
        popad
        cmp    esp, dword ptr gs:[bx + si]
        db  066h, 072h, 06Fh
        insd
        and    byte ptr [edx + edx*2 + 41h], dl
loc_2_06D0:
        inc    ebx
        inc    ebp
        push   edx
        inc    esp
        inc    esp
        inc    ebx
        add    byte ptr [edx + edx*2 + 41h], dl
        dec    esi
        push   ebx
        dec    ebp
        dec    ecx
        push   esp
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        xor    dword ptr [eax], esp
        arpl   word ptr [edi + 72h], bp
        db  072h, 075h
        db  070h, 074h
        add    byte ptr fs:[edx + edx*2 + 41h], dl
        dec    esi
        push   ebx
        dec    ebp
loc_2_06FA:
        dec    ecx
loc_2_06FB:
        push   esp
        and    byte ptr [edx + 75h], al
        db  066h, 066h, 065h, 072h, 020h
        xor    ah, byte ptr [eax]
        arpl   word ptr [edi + 72h], bp
        db  072h, 075h
        db  070h, 074h
        add    byte ptr fs:[edx + esi*2 + 79h], dl
loc_2_0713:
        imul   ebp, dword ptr [esi + 67h], 206f7420h
        db  075h, 06Eh
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        popad
        and    byte ptr [esi + 55h], cl
        dec    esp
        dec    esp
        and    byte ptr [eax + 6fh], dh
        imul   ebp, dword ptr [esi + 74h], 55007265h
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [edi + ebp*2 + 63h], ch
        imul   esp, dword ptr [eax], 70h
        outsd
        imul   ebp, dword ptr [esi + 74h], 55007265h
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ebp + 6eh], dh
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  070h, 06Fh
        db  069h, 06Eh, 074h, 065h, 072h, 000h
_PTEXT ends
        end
