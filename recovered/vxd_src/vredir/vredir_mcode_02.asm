; vredir_mcode_02.asm -- VREDIR object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        sbb    dword ptr [eax], eax
loc_2_0010:
        and    al, byte ptr [eax]
loc_2_0012:
        add    byte ptr ss:[esi], bh
        add    byte ptr [eax + eax + 76h], ah
        add    byte ptr [eax - 40ff5600h], al
        add    dl, cl
        add    ch, dh
        add    byte ptr [ecx + eax], al
loc_2_0026:
        or     al, 1
loc_2_0028:
        and    eax, dword ptr [ecx]
loc_2_002A:
        sub    al, 1
loc_2_002C:
        xor    al, 1
loc_2_002E:
        inc    edi
        add    dword ptr [ecx + 1], edx
loc_2_0032:
        pop    esi
        add    dword ptr [edx + 1], esi
loc_2_0036:
        db  07Dh, 001h
loc_2_0038:
        mov    al, byte ptr [ecx]
        cmpsb
        add    dword ptr [ebp + 69h], ecx
        arpl   word ptr [edx + 6fh], si
        db  073h, 06Fh
        db  066h, 074h, 020h
        dec    esi
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        and    byte ptr [ebx + 6ch], al
        imul   esp, dword ptr [ebp + 6eh], 4d530074h
        inc    edx
        push   edx
        imul   esi, dword ptr fs:[edx], 6e617254h
        jae    SHORT loc_2_00C5
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        db  073h, 02Fh
        jae    SHORT loc_2_00D2
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[eax + 61h], bl
        arpl   word ptr [ebx + edx*2 + 65h], si
        arpl   word ptr [eax], ax
        dec    esi
        jne    SHORT loc_2_00EA
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebx + 4dh], dl
        inc    edx
        and    byte ptr [edx + esi*2 + 61h], dh
        outsb
        db  073h, 061h
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        db  073h, 020h
        db  070h, 065h
        jb     SHORT loc_2_00B9
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 79h], al
        je     SHORT loc_2_0109
        db  073h, 020h
        db  072h, 065h
        popad
        das
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 52h], al
        popad
        jae    NEAR PTR loc_2_010C
loc_2_00B9:
        arpl   word ptr gs:[eax], ax
        inc    edx
        jns    SHORT loc_2_0133
        db  065h, 073h, 020h
        db  072h, 065h
        popad
loc_2_00C5:
        and    byte ptr fs:[esi + 72h], ah
        outsd
        insd
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
loc_2_00D2:
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_00FF
        db  073h, 065h
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 79h], al
        db  074h, 065h
loc_2_00EA:
        jae    SHORT loc_2_010C
        db  077h, 072h
        imul   esi, dword ptr [esp + esi*2 + 65h], 65732f6eh
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[edx + 57h], al
        jb     SHORT loc_2_0168
loc_2_00FF:
        je     SHORT loc_2_0166
        jae    SHORT loc_2_0156
        arpl   word ptr gs:[eax], ax
        inc    edx
        db  079h, 074h
loc_2_0109:
        jae    NEAR PTR loc_2_012C
loc_2_010C:
        db  077h, 072h
        imul   esi, dword ptr [esp + esi*2 + 65h], 6f74206eh
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + 65h], dh
        imul   esi, dword ptr fs:[edx + 65h], 726f7463h
        and    byte ptr [eax + 65h], dh
        jb     SHORT loc_2_014A
        jae    SHORT loc_2_0191
loc_2_012C:
        arpl   word ptr [edi + 6eh], bp
        add    byte ptr fs:[esi + 75h], cl
loc_2_0133:
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [esi + 65h], ch
        je     SHORT loc_2_01B2
        add    byte ptr [esi + 75h], cl
        insd
        dec    esi
        je     NEAR PTR loc_2_01BA
        add    byte ptr [esi + 75h], cl
loc_2_014A:
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [esi + 65h], ch
        je     SHORT loc_2_01C9
loc_2_0156:
        and    byte ptr [edx + 75h], dh
        outsb
        outsb
        imul   ebp, dword ptr [esi + 67h], 73655300h
        db  073h, 069h
        outsd
        outsb
loc_2_0166:
        jae    SHORT loc_2_0168
loc_2_0168:
        dec    esi
        jne    SHORT loc_2_01D8
        push   ebx
        db  065h, 073h, 073h
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [ebx + 65h], dh
        db  073h, 073h
        imul   ebp, dword ptr [edi + 6eh], 65520073h
        db  073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [esi + 75h], cl
        insd
        push   edx
loc_2_0191:
        db  065h, 073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edx + 65h], dh
        db  073h, 06Fh
        db  075h, 072h
        arpl   word ptr [ebp + 73h], sp
        add    byte ptr [edi + 70h], cl
        outsb
loc_2_01B2:
        and    byte ptr [esi + 69h], ah
        insb
        jae    NEAR PTR loc_2_01B9
loc_2_01B9:
        dec    esi
loc_2_01BA:
        db  075h, 06Dh
        dec    edi
        db  070h, 065h
        outsb
        inc    esi
        imul   ebp, dword ptr [ebp + 73h], 6d754e00h
loc_2_01C9:
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [edi + 70h], ch
        outsb
        and    byte ptr [esi + 69h], ah
        insb
loc_2_01D8:
        db  065h, 073h, 020h
        outsd
        outsb
        and    byte ptr [esi + 65h], ch
        db  074h, 000h
_LMSGTABLE ends
        end
