; vdd_mcode_03.asm -- VDD object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  005h, 000h, 000h, 000h
loc_3_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
loc_3_000E:
        xchg   ecx, eax
        add    byte ptr [ebx + 1], ah
loc_3_0012:
        and    al, 2
loc_3_0014:
        nop
        add    cl, ch
        add    dl, byte ptr [eax + ebp*2 + 65h]
        db  072h, 065h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        outsb
        outsd
        jne    SHORT loc_3_0091
        push   61766120h
        imul   ebp, dword ptr [ecx + 62h], 6d20656ch
        insd
        outsd
        db  072h, 079h
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        db  074h, 068h
        imul   esi, dword ptr [ebx + 20h], 6c707061h
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 72h], bp
        db  072h, 065h
        arpl   word ptr [esp + ebp*2 + 79h], si
        and    byte ptr [ecx + ebp*2 + 73h], ah
        db  070h, 06Ch
        popad
        db  079h, 020h
        imul   ebp, dword ptr [esi + 66h], 616d726fh
        db  074h, 069h
        outsd
        outsb
        and    byte ptr cs:[ebp + 61h], cl
        imul   esp, dword ptr [ebp + 20h], 73h
        jne    SHORT loc_3_00EE
        and    byte ptr gs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ecx + 70h], ah
        jo     SHORT loc_3_00F2
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        daa
        db  073h, 020h
        push   eax
loc_3_0091:
        db  072h, 06Fh
        jo     SHORT loc_3_00FA
        jb     SHORT loc_3_010B
        imul   esp, dword ptr [ebp + 73h], 65726120h
        and    byte ptr [ebx + 6fh], ah
        db  072h, 072h
        arpl   word ptr gs:[esi + ebp], si
        add    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        popad
        jo     SHORT loc_3_0121
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 61h], dh
        jae    SHORT loc_3_00DE
        db  075h, 06Eh
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_3_0136
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        db  074h, 068h
        and    byte ptr gs:[esi + 69h], dh
        outsd
        and    byte ptr [ecx + ebp*2 + 73h], ah
        db  070h, 06Ch
loc_3_00DE:
        popad
        db  079h, 02Eh
        and    byte ptr [ecx + 74h], cl
        db  073h, 020h
        imul   esi, dword ptr fs:[ebx + 70h], 2079616ch
loc_3_00EE:
        db  077h, 061h
        db  073h, 020h
loc_3_00F2:
        outsb
        outsd
        db  074h, 020h
        jne    SHORT loc_3_0168
        popad
loc_3_00FA:
        je     SHORT loc_3_0161
        and    byte ptr cs:[ebx + 68h], al
        arpl   word ptr gs:[ebx + 20h], bp
        je     SHORT loc_3_016F
        and    byte ptr gs:[eax + 72h], dl
loc_3_010B:
        outsd
        db  070h, 065h
        jb     SHORT loc_3_0184
        imul   esp, dword ptr [ebp + 73h], 206f7420h
        outsb
        db  073h, 075h
        jb     SHORT loc_3_0182
        and    byte ptr [eax + ebp*2 + 65h], dh
loc_3_0121:
        db  079h, 020h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        db  072h, 072h
        arpl   word ptr gs:[esi + ebp + 20h], si
        dec    ecx
        and    byte ptr [eax + ebp*2 + 65h], dh
loc_3_0136:
        and    byte ptr [ecx + ebp*2 + 73h], ah
        db  070h, 06Ch
        popad
        jns    SHORT loc_3_015F
        imul   esi, dword ptr [ebx + 20h], 61727265h
        je     SHORT loc_3_01B1
        arpl   word ptr [eax], bp
        db  072h, 065h
        jb     NEAR PTR loc_3_01B1
        db  077h, 020h
        db  074h, 068h
        and    byte ptr gs:[ecx + ebp*2 + 73h], ah
        db  070h, 06Ch
        popad
        db  079h, 020h
        outsd
loc_3_015F:
        db  072h, 020h
loc_3_0161:
        jb     SHORT loc_3_01C8
        db  073h, 074h
        popad
        jb     SHORT loc_3_01DC
loc_3_0168:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ecx + 70h], ah
loc_3_016F:
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[eax], al
        pop    ecx
        outsd
        db  075h, 020h
        arpl   word ptr [ecx + 6eh], sp
loc_3_0182:
        outsb
        outsd
loc_3_0184:
        je     SHORT loc_3_01A6
        db  072h, 075h
        outsb
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        db  067h, 072h, 061h
        db  070h, 068h
        imul   esp, dword ptr [ebx + 73h], 70706120h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 69h], dh
loc_3_01A6:
        outsb
        outsd
        ja     SHORT loc_3_0210
        and    byte ptr cs:[eax + ebp*2 + 65h], dl
loc_3_01B1:
        and    byte ptr [ecx + 70h], ah
        jo     SHORT loc_3_0222
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ebx + 75h], dh
loc_3_01C8:
        db  073h, 070h
        outsb
        and    byte ptr fs:[ebp + 6eh], dh
        db  074h, 069h
        insb
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        db  073h, 077h
loc_3_01DC:
        imul   esi, dword ptr [ebx + 68h], 20746920h
        db  074h, 06Fh
        and    byte ptr [esi + 75h], ah
        insb
        insb
        and    byte ptr [ebx + 63h], dh
        db  072h, 065h
        outsb
        and    byte ptr [edx + 79h], ah
        and    byte ptr [eax + 72h], dh
        db  065h, 073h, 073h
        imul   ebp, dword ptr [esi + 67h], 746c4120h
        sub    eax, 65746e45h
        db  072h, 02Eh
        and    byte ptr [ebx + 68h], al
        arpl   word ptr gs:[ebx + 20h], bp
loc_3_0210:
        db  074h, 068h
        and    byte ptr gs:[eax + 72h], dl
        outsd
        jo     SHORT loc_3_027E
        db  072h, 074h
        imul   esp, dword ptr [ebp + 73h], 206f7420h
loc_3_0222:
        outsb
        db  073h, 075h
        db  072h, 065h
        and    byte ptr [eax + ebp*2 + 65h], dh
        db  079h, 020h
        popad
        db  072h, 065h
        and    byte ptr [ebx + 6fh], ah
        jb     SHORT loc_3_02A8
        arpl   word ptr gs:[esi + ebp], si
        add    byte ptr [eax + ebp*2 + 69h], dl
        db  073h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ebx + 61h], ah
        outsb
        outsb
        outsd
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        jb     SHORT loc_3_02CE
        outsb
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ecx + 20h], ah
        db  077h, 069h
        outsb
        outsd
        ja     SHORT loc_3_0295
        and    byte ptr [eax + 72h], dl
        db  065h, 073h, 073h
        and    byte ptr [ecx + 6eh], ah
        db  079h, 020h
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        outsb
        jbe    SHORT loc_3_02E3
loc_3_027E:
        db  072h, 074h
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + ebp*2 + 20h], dh
loc_3_0295:
        db  066h, 075h, 06Ch
        insb
        and    byte ptr [ebx + 63h], dh
        db  072h, 065h
        outsb
        and    byte ptr [ebp + 6fh], ch
        add    byte ptr cs:[eax], al
loc_3_02A8:
        push   esp
        push   61207369h
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ebx + 61h], ah
        outsb
        outsb
        outsd
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        db  072h, 065h
        db  073h, 074h
        outsd
        db  072h, 065h
        and    byte ptr fs:[ecx + 6eh], ah
loc_3_02CE:
        and    byte ptr fs:[edi + 69h], dh
        insb
        insb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ebp + 72h], dh
        insd
        imul   ebp, dword ptr [esi + 61h], 2e646574h
loc_3_02E3:
        add    byte ptr [eax], al
_LMSGTABLE ends
        end
