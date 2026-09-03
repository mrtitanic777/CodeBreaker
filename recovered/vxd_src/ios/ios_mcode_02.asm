; ios_mcode_02.asm -- IOS object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  003h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        je     SHORT loc_2_0010
loc_2_0010:
        inc    dword ptr [eax]
        push   ebx
        add    dword ptr [ebp + 72h], eax
        db  072h, 06Fh
        jb     SHORT loc_2_0054
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [ebx + 75h], dh
        bound  esi, qword ptr [ebx + 79h]
        db  073h, 074h
        insd
        and    byte ptr [esi + 69h], ah
        insb
        jae    NEAR PTR loc_2_004F
        insd
        imul   esi, dword ptr [ebx + 73h], 20676e69h
        outsd
        db  072h, 020h
        pop    esp
        imul   ebp, dword ptr [edi + 73h], 79736275h
        jae    SHORT loc_2_0065
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        db  079h, 020h
loc_2_004F:
        or     eax, 746f6e0ah
loc_2_0054:
        and    byte ptr [esi + 6fh], ah
        db  075h, 06Eh
        or     eax, 6e75520ah
        and    byte ptr [ecx + 4eh], cl
        push   ebx
        push   esp
loc_2_0065:
        inc    ecx
        dec    esp
        dec    esp
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 20h], 63206f74h
        outsd
        db  072h, 072h
        arpl   word ptr gs:[eax + 74h], si
        push   72702065h
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        add    byte ptr cs:[eax], al
        inc    ebp
        jb     SHORT loc_2_00FD
        outsd
        jb     SHORT loc_2_00C8
        and    byte ptr [ecx + 6eh], al
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [ebx + 75h], dh
        bound  esi, qword ptr [ebx + 79h]
        db  073h, 074h
        insd
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
        popaw
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        insb
        outsd
        popad
        or     eax, 7469450ah
        push   61207265h
        and    byte ptr [esi + 69h], ah
        insb
        and    byte ptr gs:[ecx + 6eh], ch
loc_2_00C8:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [esi], ch
        pop    esp
        imul   ebp, dword ptr [edi + 73h], 79736275h
        db  073h, 020h
        db  073h, 075h
        bound  esp, qword ptr [ecx + ebp*2 + 72h]
        arpl   word ptr gs:[edi + ebp*2 + 72h], si
        db  079h, 020h
        imul   esi, dword ptr [ebx + 20h], 72726f63h
        jne    SHORT loc_2_015E
        db  074h, 02Ch
        or     eax, 20726f0ah
        db  074h, 068h
        and    byte ptr gs:[ebx + 79h], dh
        db  073h, 074h
loc_2_00FD:
        insd
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [edi + ebp*2 + 77h], ch
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        add    byte ptr cs:[eax], al
        inc    ebp
        db  072h, 072h
        outsd
        db  072h, 03Ah
        and    byte ptr [eax + ebp*2 + 65h], dl
        and    byte ptr [ecx + 2fh], cl
        dec    edi
        and    byte ptr [ebx + 75h], dh
        bound  esi, qword ptr [ebx + 79h]
        db  073h, 074h
        insd
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        popad
        insb
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[esi + 65h], ch
        and    byte ptr fs:[ebp + 65h], ch
        insd
        outsd
        db  072h, 079h
        or     eax, 756f590ah
        and    byte ptr [esi + 65h], ch
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 075h
        outsb
loc_2_015E:
        and    byte ptr [ebx + 65h], dh
        db  074h, 075h
        db  070h, 02Eh
        add    byte ptr [eax], al
_LMSGTABLE ends
        end
