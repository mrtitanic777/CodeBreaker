; vpicd_mcode_02.asm -- VPICD object 2 segment _LMSGTABLE
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
        or     dword ptr [ecx], eax
        dec    ebx
        add    dword ptr [eax], eax
        db  000h
loc_2_0014:
        push   esp
        push   61207369h
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ecx + 74h], ah
        je     SHORT loc_2_008E
        insd
        db  070h, 074h
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        popad
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        and    byte ptr [ecx + 20h], ah
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        je     SHORT loc_2_00AC
        popad
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 6e696562h
        and    byte ptr [di + 73h], dh
        and    byte ptr fs:[edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        jb     NEAR PTR loc_2_0080
        popad
        jo     SHORT loc_2_00D3
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr cs:[ecx + 66h], cl
        and    byte ptr [ecx + 6fh], bh
        db  075h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 069h
        outsb
        db  075h, 065h
        and    byte ptr [edx + 75h], dh
        outsb
loc_2_0080:
        outsb
        imul   ebp, dword ptr [esi + 67h], 69687420h
        db  073h, 020h
        popad
        db  070h, 070h
        insb
loc_2_008E:
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr [ecx + 74h], ch
        and    byte ptr [ebp + 69h], ch
        push   61632074h
        db  075h, 073h
        and    byte ptr gs:[ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 79h], dh
loc_2_00AC:
        db  073h, 074h
        insd
        and    byte ptr [edi + ebp*2 + 20h], dh
        insb
        outsd
        arpl   word ptr [ebx + 20h], bp
        db  075h, 070h
        or     eax, 79540a0ah
        db  070h, 065h
        and    byte ptr [ecx + 20h], bl
        outsd
        db  072h, 020h
        db  070h, 072h
        db  065h, 073h, 073h
        and    byte ptr [ebp + 4eh], al
        push   esp
        inc    ebp
loc_2_00D3:
        push   edx
        and    byte ptr [edi + ebp*2 + 20h], dh
        arpl   word ptr [edi + 6eh], bp
        je     SHORT loc_2_0146
        outsb
        jne    SHORT loc_2_0145
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
        imul   ebp, dword ptr [esi + 67h], 69687420h
        db  073h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        or     eax, 7079540ah
        and    byte ptr gs:[esi + 20h], cl
        db  074h, 06Fh
        and    byte ptr [ebp + 6eh], ah
        and    byte ptr fs:[eax + ebp*2 + 69h], dh
        db  073h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[eax], al
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        arpl   word ptr [edi + 6eh], bp
        insb
        imul   esp, dword ptr [ebx + 74h], 7551202eh
        imul   esi, dword ptr [eax + 61h], 61206c6ch
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        db  073h, 020h
        popad
loc_2_0145:
        outsb
loc_2_0146:
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        outsb
        and    byte ptr [edx + 65h], dh
        db  073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        add    byte ptr cs:[eax], al
_LMSGTABLE ends
        end
