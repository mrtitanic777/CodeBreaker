; vwin32_mcode_02.asm -- VWIN32 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        sub    al, byte ptr [edx]
        add    byte ptr [eax], al
loc_2_0014:
        inc    ecx
        and    byte ptr [esi + 61h], ah
        db  074h, 061h
        insb
        and    byte ptr [ebp + 78h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_008C
        outsd
        outsb
        and    byte ptr [20783231h], ah
        push   6f207361h
        arpl   word ptr [ebx + 75h], sp
        db  072h, 072h
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [3a783432h], ah
        and    eax, 2e783833h
        and    byte ptr [eax], ah
        push   esp
        push   75632065h
        db  072h, 072h
        outsb
        db  074h, 020h
        popad
        jo     SHORT loc_2_00C6
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ebp + 72h], dh
        insd
        imul   ebp, dword ptr [esi + 61h], 2e646574h
        or     eax, 202a0a0ah
        and    byte ptr [eax + 72h], dl
        jae    NEAR PTR loc_2_00F0
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_2_00A2
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebp + 72h], dh
loc_2_008C:
        insd
        imul   ebp, dword ptr [esi + 61h], 74206574h
        push   75632065h
        jb     SHORT loc_2_010D
        outsb
        db  074h, 020h
        popad
        db  070h, 070h
loc_2_00A2:
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        or     eax, 20202a0ah
        push   eax
        db  072h, 065h
        jae    SHORT loc_2_0128
        and    byte ptr [ebx + 54h], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 4ch]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 4ch]
        and    byte ptr [ecx + 67h], ah
        popad
loc_2_00C6:
        imul   ebp, dword ptr [esi + 20h], 72206f74h
        db  065h, 073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        jne    SHORT loc_2_014A
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        je     SHORT loc_2_0145
        jb     SHORT loc_2_0110
        and    byte ptr [ecx + 6fh], bl
        db  075h, 020h
        db  077h, 069h
        insb
        insb
        or     eax, 2020080ah
loc_2_00F0:
        and    byte ptr [edi + ebp*2 + 73h], ch
        and    byte ptr gs:[ecx + 6eh], ah
        db  079h, 020h
        db  075h, 06Eh
        db  073h, 061h
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ch
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
loc_2_010D:
        and    byte ptr [ecx + 6eh], ch
loc_2_0110:
        and    byte ptr [ecx + 6ch], ah
        insb
        and    byte ptr [ecx + 70h], ah
        jo     SHORT loc_2_0185
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        jae    SHORT loc_2_0150
        add    byte ptr [eax], al
        inc    ecx
        and    byte ptr [esi + 61h], ah
loc_2_0128:
        db  074h, 061h
        insb
        and    byte ptr [ebp + 78h], ah
        arpl   word ptr [ebp + 70h], sp
        je     SHORT loc_2_019C
        outsd
        outsb
        and    byte ptr [20783231h], ah
        push   6f207361h
        arpl   word ptr [ebx + 75h], sp
        jb     SHORT loc_2_01B7
loc_2_0145:
        and    byte ptr fs:[ecx + 74h], ah
loc_2_014A:
        and    byte ptr [3a783432h], ah
loc_2_0150:
        and    eax, 20783833h
        imul   ebp, dword ptr [esi + 20h], 20445856h
        and    eax, 202e7334h
        push   esp
        push   75632065h
        db  072h, 072h
        outsb
        db  074h, 020h
        popad
        jo     SHORT loc_2_01E0
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ebp + 72h], dh
        insd
loc_2_0185:
        imul   ebp, dword ptr [esi + 61h], 2e646574h
        or     eax, 202a0a0ah
        and    byte ptr [eax + 72h], dl
        jae    NEAR PTR loc_2_020A
        and    byte ptr [ecx + 6eh], ah
        jns    SHORT loc_2_01BC
loc_2_019C:
        imul   esp, dword ptr [ebp + 79h], 20h
        db  074h, 06Fh
        and    byte ptr [ebp + 72h], dh
        insd
        imul   ebp, dword ptr [esi + 61h], 74206574h
        push   75632065h
        jb     SHORT loc_2_0227
        outsb
loc_2_01B7:
        db  074h, 020h
        popad
        db  070h, 070h
loc_2_01BC:
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        or     eax, 20202a0ah
        push   eax
        db  072h, 065h
        db  073h, 073h
        and    byte ptr [ebx + 54h], al
        push   edx
        dec    esp
        sub    eax, dword ptr [ecx + 4ch]
        push   esp
        sub    eax, dword ptr [ebp + eax*2 + 4ch]
        and    byte ptr [ecx + 67h], ah
        popad
loc_2_01E0:
        imul   ebp, dword ptr [esi + 20h], 72206f74h
        db  065h, 073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        db  074h, 065h
        jb     SHORT loc_2_022A
        and    byte ptr [ecx + 6fh], bl
        db  075h, 020h
        db  077h, 069h
        insb
        insb
        or     eax, 2020080ah
loc_2_020A:
        and    byte ptr [edi + ebp*2 + 73h], ch
        and    byte ptr gs:[ecx + 6eh], ah
        db  079h, 020h
        db  075h, 06Eh
        db  073h, 061h
        db  076h, 065h
        and    byte ptr fs:[ecx + 6eh], ch
        outsw
        db  072h, 06Dh
        popad
        db  074h, 069h
        outsd
        outsb
loc_2_0227:
        and    byte ptr [ecx + 6eh], ch
loc_2_022A:
        and    byte ptr [ecx + 6ch], ah
        insb
        and    byte ptr [ecx + 70h], ah
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        db  073h, 02Eh
        add    byte ptr [eax], al
_LMSGTABLE ends
        end
