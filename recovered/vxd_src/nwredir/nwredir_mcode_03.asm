; nwredir_mcode_03.asm -- NWREDIR object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  01Dh, 000h, 000h, 000h
loc_3_0004:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [esi], ch
        add    byte ptr [eax], al
loc_3_0014:
        dec    edx
        add    byte ptr [eax + eax - 77h], dh
        add    byte ptr [eax + eax + 0ce00bfh], dl
loc_3_0020:
        fiadd  dword ptr [eax]
loc_3_0022:
        sbb    eax, 43013201h
        add    dword ptr [esi - 3afe45ffh], esp
        add    ebx, esi
        add    dword ptr [ecx], ecx
        add    dl, byte ptr [esi]
        add    al, byte ptr [edi + 2]
loc_3_0036:
        pop    eax
        add    ah, byte ptr [eax + 2]
loc_3_003A:
        cdq
        add    ch, byte ptr [ebp - 0ffd4afeh]
        add    al, byte ptr [edi]
        add    ecx, dword ptr [edi]
        add    ecx, dword ptr [edx + 3]
loc_3_0048:
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
        je     SHORT loc_3_0072
        inc    ebx
        insb
        imul   esp, dword ptr [ebp + 6eh], 6f662074h
        db  072h, 020h
        dec    esi
        db  065h, 074h, 057h
        popad
        db  072h, 065h
        and    byte ptr [esi + 65h], cl
        db  074h, 077h
        outsd
        jb     SHORT loc_3_00D7
        jae    SHORT loc_3_006E
loc_3_006E:
        dec    esi
        push   edi
        push   edx
        inc    ebp
loc_3_0072:
        inc    esp
        dec    ecx
        push   edx
        add    byte ptr [edx + 79h], al
        je     SHORT loc_3_00DF
        db  073h, 020h
        db  072h, 065h
        popad
        das
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 52h], al
        popad
        jae    NEAR PTR loc_3_00E2
        arpl   word ptr gs:[eax], ax
        inc    edx
        jns    SHORT loc_3_0109
        db  065h, 073h, 020h
        db  072h, 065h
        popad
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_3_00D5
        jae    SHORT loc_3_011C
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 79h], al
        je     SHORT loc_3_0125
        jae    SHORT loc_3_00E2
        ja     SHORT loc_3_0136
        imul   esi, dword ptr [esp + esi*2 + 65h], 65732f6eh
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 57h], al
        db  072h, 069h
loc_3_00D5:
        je     SHORT loc_3_013C
loc_3_00D7:
        db  073h, 053h
        arpl   word ptr gs:[eax], ax
        inc    edx
        db  079h, 074h
loc_3_00DF:
        jae    NEAR PTR loc_3_0102
loc_3_00E2:
        ja     SHORT loc_3_0156
        imul   esi, dword ptr [esp + esi*2 + 65h], 6f74206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_3_0120
        db  073h, 065h
loc_3_0102:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 79h], al
loc_3_0109:
        db  074h, 065h
        db  073h, 020h
        imul   ebp, dword ptr [esi + 20h], 68636163h
        add    byte ptr gs:[edx + 79h], al
        je     SHORT loc_3_017F
        db  073h, 043h
loc_3_011C:
        popad
        arpl   word ptr [eax + 65h], bp
loc_3_0120:
        add    byte ptr fs:[ecx + 6dh], al
        outsd
loc_3_0125:
        db  075h, 06Eh
        db  074h, 020h
        outsd
        and    byte ptr [ecx + 74h], ah
        popad
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [edx + 79h], ah
loc_3_0136:
        db  074h, 065h
        db  073h, 020h
        db  074h, 068h
loc_3_013C:
        popad
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 72727563h
        outsb
        db  074h, 06Ch
        db  079h, 020h
        arpl   word ptr [ecx + 63h], sp
        push   62206465h
        db  079h, 020h
loc_3_0156:
        db  074h, 068h
        and    byte ptr gs:[edx + 65h], dh
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        add    byte ptr [ecx + ebp*2 + 72h], al
        db  074h, 079h
        and    byte ptr [edx + 79h], ah
        db  074h, 065h
        db  073h, 020h
        imul   ebp, dword ptr [esi + 20h], 68636163h
        add    byte ptr gs:[ecx + ebp*2 + 72h], al
        db  074h, 079h
loc_3_017F:
        inc    edx
        db  079h, 074h
        db  065h, 073h, 043h
        popad
        arpl   word ptr [eax + 65h], bp
        add    byte ptr fs:[ecx + 6dh], al
        outsd
        db  075h, 06Eh
        db  074h, 020h
        outsd
        and    byte ptr [ecx + ebp*2 + 72h], ah
        je     SHORT loc_3_0213
        and    byte ptr [ecx + 74h], ah
        popad
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [edx + 79h], ah
        je     SHORT loc_3_020C
        db  073h, 020h
        je     SHORT loc_3_0213
        popad
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 72727563h
        outsb
        je     SHORT loc_3_0225
        db  079h, 020h
        arpl   word ptr [ecx + 63h], sp
        push   62206465h
        jns    SHORT loc_3_01E5
        db  074h, 068h
        and    byte ptr gs:[edx + 65h], dh
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edi + 61h], dh
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 6f742067h
        and    byte ptr [edx + 65h], ah
loc_3_01E5:
        and    byte ptr [edi + 72h], dh
        imul   esi, dword ptr [esp + esi*2 + 65h], 434e006eh
        push   eax
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_3_026C
        and    byte ptr [edx + esi*2 + 6fh], ah
        db  070h, 070h
        add    byte ptr fs:[edx + esi*2 + 6fh], al
        db  070h, 070h
        dec    esi
        inc    ebx
        push   eax
loc_3_020C:
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
loc_3_0213:
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 65h], dh
        db  067h, 075h, 06Ch
        popad
        jb     SHORT loc_3_023F
        dec    esi
        inc    ebx
        push   eax
        and    byte ptr [eax + 61h], dh
loc_3_0225:
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 73h], ch
        db  074h, 020h
        imul   ebp, dword ptr [esi + 20h], 6e617274h
        db  073h, 069h
        je     SHORT loc_3_023B
loc_3_023B:
        inc    edx
        push   ebp
        push   edx
        push   ebx
loc_3_023F:
        push   esp
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_3_02BB
        and    byte ptr [edx + esi*2 + 6fh], ah
        db  070h, 070h
        add    byte ptr fs:[edx + esi*2 + 6fh], al
        db  070h, 070h
        inc    edx
        push   ebp
        push   edx
        push   ebx
        push   esp
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 75h], ah
        db  072h, 073h
loc_3_026C:
        db  074h, 020h
        jo     SHORT loc_3_02E2
        outsd
        je     SHORT loc_3_02E2
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [eax + 61h], dh
        arpl   word ptr [ebx + 65h], bp
        db  074h, 073h
        and    byte ptr [edi + ebp*2 + 73h], ch
        je     SHORT loc_3_02A4
        imul   ebp, dword ptr [esi + 20h], 6e617274h
        db  073h, 069h
        je     SHORT loc_3_028F
loc_3_028F:
        push   edx
        db  065h, 071h, 075h
        db  065h, 073h, 074h
        db  073h, 020h
        jo     SHORT loc_3_02FF
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 71655200h
        push   eax
loc_3_02A4:
        outsb
        add    byte ptr fs:[esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 65h], dh
        jno    SHORT loc_3_032B
        db  065h, 073h, 074h
        db  073h, 020h
loc_3_02BB:
        db  077h, 061h
        imul   esi, dword ptr [ecx + ebp*2 + 6eh], 6f742067h
        and    byte ptr [edx + 65h], ah
        and    byte ptr [eax + 72h], dh
        outsd
        arpl   word ptr [ebp + 73h], sp
        db  073h, 065h
        and    byte ptr fs:[edx + 79h], ah
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dh
        jb     SHORT loc_3_0354
        jb     NEAR PTR loc_3_02E1
loc_3_02E1:
        inc    edx
loc_3_02E2:
        db  075h, 072h
        db  073h, 074h
        and    byte ptr [ebx + 65h], dh
        outsb
        and    byte ptr fs:[edi + 61h], ah
        db  070h, 020h
        je     SHORT loc_3_035B
        insd
        add    byte ptr gs:[ebx + 65h], dl
        outsb
        inc    edi
        popad
        jo     SHORT loc_3_02FD
loc_3_02FD:
        dec    ecx
        outsb
loc_3_02FF:
        db  074h, 065h
        jb     SHORT loc_3_0373
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
        popad
        db  070h, 020h
        outsw
        db  072h, 020h
        outsd
        db  075h, 074h
        sub    eax, 6e696f67h
        and    byte ptr [bp + si + 75h], ah
        db  072h, 073h
        db  074h, 020h
        db  074h, 072h
        popad
        imul   sp, word ptr [ebx + 2ch], 6d20h
loc_3_032B:
        imul   esp, dword ptr [ebx + 72h], 65732d6fh
        arpl   word ptr [edi + 6eh], bp
        jae    NEAR PTR loc_3_0338
loc_3_0338:
        inc    edx
        db  075h, 072h
        db  073h, 074h
        and    byte ptr [edx + 65h], dh
        arpl   word ptr [ebp + 69h], sp
        db  076h, 065h
        and    byte ptr [edi + 61h], ah
        db  070h, 020h
        db  074h, 069h
        insd
        add    byte ptr gs:[edx + 65h], dl
        arpl   word ptr [esi + 47h], si
loc_3_0354:
        popad
        jo     SHORT loc_3_0357
loc_3_0357:
        dec    ecx
        outsb
        db  074h, 065h
loc_3_035B:
        db  072h, 070h
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 020h
        popad
        db  070h, 020h
        outsw
        db  072h, 020h
        imul   ebp, dword ptr [esi + 2dh], 696d6f63h
        outsb
loc_3_0373:
        and    byte ptr [bp + si + 75h], ah
        db  072h, 073h
        db  074h, 020h
        db  074h, 072h
        popad
        imul   sp, word ptr [ebx + 2ch], 6d20h
        imul   esp, dword ptr [ebx + 72h], 65732d6fh
        arpl   word ptr [edi + 6eh], bp
        db  064h, 073h, 000h
_LMSGTABLE ends
        end
