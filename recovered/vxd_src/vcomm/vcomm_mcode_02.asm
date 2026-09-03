; vcomm_mcode_02.asm -- VCOMM object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        add    al, 0
        add    byte ptr [eax], al
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_000E:
        pop    edi
        add    byte ptr [eax], dh
loc_2_0012:
        int3
        add    dh, ch
        add    byte ptr [eax], al
        add    byte ptr [eax + ebp*2 + 65h], dl
        and    byte ptr [ebx + 4fh], al
        dec    ebp
        and    eax, 6f702075h
        db  072h, 074h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 73h], ah
        db  073h, 069h
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        popad
        and    byte ptr [edi + ecx*2 + 53h], al
        and    byte ptr [ecx + 70h], ah
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr cs:[edi + ebp*2 + 20h], al
        jns    SHORT loc_2_00BD
        db  075h, 020h
        db  077h, 061h
        outsb
        je     SHORT loc_2_0075
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dh
        popad
        db  073h, 073h
        imul   esp, dword ptr [edi + 6eh], 65687420h
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   edi
        imul   ebp, dword ptr [esi + 64h], 3f73776fh
loc_2_0075:
        add    byte ptr [eax], al
        inc    esp
        jbe    NEAR PTR loc_2_00E4
        arpl   word ptr [ebp + 20h], sp
        inc    ebx
        outsd
        outsb
        insb
        imul   esp, dword ptr [ebx + 74h], 68540000h
        and    byte ptr gs:[6f702073h], ah
        db  072h, 074h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 73h], ah
        jae    SHORT loc_2_0104
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        popad
        and    byte ptr [edi + ecx*2 + 53h], al
        and    byte ptr [ecx + 70h], ah
        db  070h, 06Ch
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        and    byte ptr cs:[edi + ebp*2 + 20h], al
        db  079h, 06Fh
        db  075h, 020h
loc_2_00BD:
        db  077h, 061h
        outsb
        je     SHORT loc_2_00E2
        db  074h, 06Fh
        and    byte ptr [edx + 65h], dh
        popad
        db  073h, 073h
        imul   esp, dword ptr [edi + 6eh], 65687420h
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [edi + ebp*2 + 20h], dh
        push   edi
        imul   ebp, dword ptr [esi + 64h], 3f73776fh
loc_2_00E2:
        add    byte ptr [eax], al
loc_2_00E4:
        dec    ecx
        db  073h, 020h
        imul   esi, dword ptr [eax + 6fh], 6f74206bh
        and    byte ptr [edx + 65h], dh
        insd
        outsd
        db  076h, 065h
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        aas
loc_2_0104:
        add    byte ptr [eax], al
_LMSGTABLE ends
        end
