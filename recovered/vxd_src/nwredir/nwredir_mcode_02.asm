; nwredir_mcode_02.asm -- NWREDIR object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  014h, 000h, 000h, 000h
loc_2_0004:
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
loc_2_000A:
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        sbb    eax, dword ptr [eax]
loc_2_0010:
        fadd   dword ptr [eax]
loc_2_0012:
        out    0, al
loc_2_0014:
        stc
        add    byte ptr [edi], dh
        add    dword ptr [ebx + 1], eax
loc_2_001A:
        push   edx
        db  001h
loc_2_001C:
        pushad
        add    dword ptr [edx + 1], ebp
        db  075h, 001h
        add    byte ptr [ecx], 8dh
        add    dword ptr [ecx - 3efe4affh], esp
        add    edi, ecx
        add    edx, esp
        add    esi, esi
        add    dword ptr [edi], eax
        add    dl, byte ptr [edx + eax]
        add    byte ptr [eax], al
loc_2_0038:
        inc    ebx
        insb
        imul   esp, dword ptr [ebp + 6eh], 65532074h
        db  072h, 076h
        imul   esp, dword ptr [ebx + 65h], 726f6620h
        and    byte ptr [esi + 65h], cl
        db  074h, 077h
        popad
        jb     SHORT loc_2_00B7
        add    byte ptr [ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 06Ch
        je     NEAR PTR loc_2_00CA
        and    byte ptr [ecx + 20h], ah
        bound  esp, qword ptr [ecx + 63h]
        imul   esp, dword ptr [edi + 72h], 6fh
        db  075h, 06Eh
        and    byte ptr fs:[edi + 70h], ch
        jb     NEAR PTR loc_2_00D9
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edi + 6eh], ch
        and    byte ptr [ebx + 6fh], ah
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        and    byte ptr [edi + ebp*2 + 20h], dh
        and    eax, 20202e73h
        push   ebx
        insb
        arpl   word ptr gs:[eax + 52h], si
        db  065h, 074h, 072h
        db  079h, 020h
        db  074h, 06Fh
        and    byte ptr [edx + esi*2 + 79h], dh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edi + 70h], ch
        db  065h, 072h, 061h
        je     SHORT loc_2_011B
        outsd
        outsb
        and    byte ptr [ecx + 67h], ah
loc_2_00B7:
        popad
        imul   ebp, dword ptr [esi + 2ch], 20726f20h
        db  070h, 072h
        db  065h, 073h, 073h
        and    byte ptr [ebp + 73h], al
        arpl   word ptr [ecx + 70h], sp
loc_2_00CA:
        and    byte ptr gs:[edi + ebp*2 + 20h], dh
        arpl   word ptr [ecx + 6eh], sp
        arpl   word ptr [ebp + 6ch], sp
        and    byte ptr [ecx + 6ch], ah
        insb
loc_2_00D9:
        and    byte ptr [edi + 75h], ch
        db  074h, 073h
        db  074h, 061h
        outsb
        imul   ebp, dword ptr fs:[esi + 67h], 63616220h
        imul   esp, dword ptr [edi + 72h], 6fh
        jne    SHORT loc_2_015D
        and    byte ptr fs:[edi + 70h], ch
        db  065h, 072h, 061h
        je     SHORT loc_2_0161
        outsd
        outsb
        db  073h, 020h
        outsd
        outsb
        and    byte ptr [eax + ebp*2 + 69h], dh
        jae    SHORT loc_2_0124
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        add    byte ptr cs:[edi + 70h], cl
        jb     NEAR PTR loc_2_0176
        db  074h, 069h
        outsd
        outsb
        cmp    ah, byte ptr [eax]
loc_2_011B:
        and    eax, 72450073h
        jb     SHORT loc_2_0191
        jb     SHORT loc_2_015E
loc_2_0124:
        and    byte ptr [eax], dh
        db  078h, 025h
        xor    byte ptr [edx], dh
        db  078h, 020h
        daa
        and    eax, 53002773h
        insb
        arpl   word ptr gs:[eax + 5bh], si
        push   edx
        inc    ebp
        push   esp
        push   edx
        pop    ecx
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  072h, 065h
        db  074h, 072h
        db  079h, 020h
        db  074h, 068h
        and    byte ptr gs:[edi + 70h], ch
        db  065h, 072h, 061h
        db  074h, 069h
        outsd
        outsb
        sub    al, 20h
        outsd
        db  072h, 020h
        pop    ebx
        dec    ecx
loc_2_015D:
        inc    edi
loc_2_015E:
        dec    esi
        dec    edi
        push   edx
loc_2_0161:
        inc    ebp
        pop    ebp
        and    byte ptr [edi + ebp*2 + 20h], dh
        imul   esi, dword ptr fs:[ebx + 63h], 647261h
        dec    esi
        outsd
        and    byte ptr [edx + 65h], dh
        jae    SHORT loc_2_01E6
loc_2_0176:
        outsd
        outsb
        db  073h, 065h
        add    byte ptr [edx + 61h], al
        and    byte ptr fs:[ebx + 6fh], ah
        outsb
        outsb
        arpl   word ptr gs:[ecx + ebp*2 + 6fh], si
        outsb
        add    byte ptr [ebp + 6eh], dl
        imul   ebp, dword ptr [esi + 6fh], 77h
        outsb
loc_2_0191:
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        jb     SHORT loc_2_0198
loc_2_0198:
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 64616552h
        add    byte ptr [esi + 69h], al
        insb
        and    byte ptr gs:[edi + 72h], dl
        imul   esi, dword ptr [ebp], 656c6946h
        and    byte ptr [ebx + 6ch], al
        outsd
        db  073h, 065h
        add    byte ptr [eax + 72h], dl
        outsd
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        inc    ebp
        db  078h, 069h
        je     SHORT loc_2_01C5
loc_2_01C5:
        inc    edi
        db  065h, 074h, 020h
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 72747441h
        imul   esp, dword ptr [edx + 75h], 736574h
        inc    edi
        db  065h, 074h, 020h
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 72747441h
loc_2_01E6:
        imul   esp, dword ptr [edx + 75h], 736574h
        push   edx
        arpl   word ptr gs:[edi + 72h], bp
        and    byte ptr fs:[edi + ebp*2 + 63h], cl
        imul   eax, dword ptr [eax], 52h
        arpl   word ptr gs:[edi + 72h], bp
        and    byte ptr fs:[ebp + 6eh], dl
        insb
        outsd
        arpl   word ptr [ebx], bp
        push   ebx
        db  065h, 073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 73694420h
        arpl   word ptr [edi + 6eh], bp
        outsb
        arpl   word ptr gs:[eax + eax + 53h], si
        je     NEAR PTR loc_2_023E
        inc    esi
        imul   ebp, dword ptr [ebp + 20h], 72747441h
        imul   esp, dword ptr [edx + 75h], 736574h
        inc    edi
        db  065h, 074h, 020h
        push   esi
        outsd
        insb
        db  075h, 06Dh
        and    byte ptr gs:[ebx + 74h], dl
        popad
        db  074h, 073h
loc_2_023E:
        add    byte ptr [edi + 65h], al
        db  074h, 020h
        inc    esp
        imul   esi, dword ptr [edx + 20h], 68746150h
        db  000h
_LMSGTABLE ends
        end
