; voltrack_mcode_02.asm -- VOLTRACK object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0004:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        pop    ebp
        add    byte ptr [edi], bh
        inc    dword ptr [eax]
loc_2_0016:
        and    al, 1
loc_2_0018:
        inc    ebx
        db  001h
loc_2_001A:
        sahf
        add    edi, ebx
        add    dword ptr [eax], eax
        add    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        jb     SHORT loc_2_0046
        push   edx
        popad
        imul   ebp, dword ptr fs:[esi + 67h], 73694420h
        imul   esp, dword ptr [eax], 69h
        outsb
        and    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_2_00A0
        and    byte ptr [50000073h], ah
        insb
        popad
        db  073h, 065h
loc_2_0046:
        and    byte ptr [ecx + 6eh], cl
        db  073h, 065h
        db  072h, 074h
        and    byte ptr [ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 25h
        xor    dword ptr [ebx + 0dh], esi
        or     dl, byte ptr [edi + 69h]
        je     SHORT loc_2_00C4
        and    byte ptr [ebx + 65h], dl
        db  072h, 069h
        popad
        insb
        and    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [69207332h], ah
        outsb
        and    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        and    byte ptr [7333h], ah
        inc    ebp
        db  072h, 072h
        outsd
        jb     SHORT loc_2_00A3
        push   edx
        popad
        imul   ebp, dword ptr fs:[esi + 67h], 2d444320h
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [edx + esi*2 + 69h], al
        db  076h, 065h
        and    byte ptr [50000073h], ah
loc_2_00A0:
        insb
        popad
loc_2_00A3:
        db  073h, 065h
        and    byte ptr [ecx + 6eh], cl
        db  073h, 065h
        jb     SHORT loc_2_0120
        and    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        and    eax, 0a0d7331h
        push   edi
        imul   esi, dword ptr [eax + ebp*2 + 20h], 69726553h
        popad
        insb
loc_2_00C4:
        and    byte ptr [esi + 75h], cl
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [69207332h], ah
        outsb
        and    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_2_013D
        and    byte ptr [0a0d7333h], ah
        or     eax, 2066490ah
        db  074h, 068h
        and    byte ptr gs:[ebx + 44h], al
        sub    eax, 204d4f52h
        imul   esi, dword ptr [ebx + 20h], 6c697473h
        insb
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edx + esi*2 + 69h], ah
        jbe    SHORT loc_2_0168
        sub    al, 20h
        imul   esi, dword ptr [eax + 6dh], 72207961h
        jno    NEAR PTR loc_2_0185
        imul   esi, dword ptr [edx + 65h], 656c6320h
        popad
        outsb
        imul   ebp, dword ptr [esi + 67h], 5000002eh
loc_2_0120:
        insb
        popad
        jae    SHORT loc_2_018A
        and    byte ptr [ecx + 6eh], cl
        db  073h, 065h
        db  072h, 074h
        and    byte ptr [ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [ebp + 74h], 74h
        and    byte ptr gs:[esi + 6fh], ah
        jb     SHORT loc_2_015A
        inc    esp
        jb     SHORT loc_2_01A6
loc_2_013D:
        jbe    SHORT loc_2_01A4
        and    byte ptr [45000073h], ah
        push   65h
        arpl   word ptr [eax + 52h], si
        db  065h, 071h, 075h
        db  065h, 073h, 074h
        and    byte ptr [edi + ebp*2 + 20h], dh
        inc    esp
        db  072h, 069h
        jbe    SHORT loc_2_01BF
loc_2_015A:
        and    byte ptr [ecx + 6eh], cl
        and    byte ptr [ebp + 73h], dl
        add    byte ptr gs:[eax], al
        inc    ecx
        outsb
        and    byte ptr [ebp + 4ah], al
loc_2_0168:
        inc    ebp
        inc    ebx
        push   esp
        and    byte ptr [edx + 65h], dh
        db  071h, 075h
        jae    NEAR PTR loc_2_01E7
        and    byte ptr [edi + 61h], dh
        db  073h, 020h
        jb     SHORT loc_2_01DF
        arpl   word ptr [ebp + 69h], sp
        jbe    SHORT loc_2_01E4
        and    byte ptr fs:[esi + 6fh], ah
        db  072h, 020h
loc_2_0185:
        popad
        and    byte ptr [edx + esi*2 + 69h], ah
loc_2_018A:
        jbe    SHORT loc_2_01F1
        and    byte ptr [eax + ebp*2 + 61h], dh
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 75206e69h
        db  073h, 065h
        or     eax, 6e6f430ah
        db  074h, 069h
        outsb
loc_2_01A4:
        db  075h, 065h
loc_2_01A6:
        and    byte ptr [edi + 69h], dh
        db  074h, 068h
        and    byte ptr [ebp + 6ah], ah
        arpl   word ptr gs:[eax + 6fh], si
        db  070h, 065h
        db  072h, 061h
        db  074h, 069h
        outsd
        outsb
        aas
        add    byte ptr [eax], al
        push   esp
loc_2_01BF:
        push   72642065h
        imul   esi, dword ptr [esi + 65h], 6e616320h
        outsb
        outsd
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        push   65h
        arpl   word ptr [ebp + 64h], si
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
loc_2_01DF:
        db  073h, 065h
        and    byte ptr [ecx + 74h], ch
loc_2_01E4:
        and    byte ptr [ecx + 73h], ch
loc_2_01E7:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
loc_2_01F1:
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 79h], dh
        db  073h, 074h
        insd
        add    byte ptr cs:[eax], al
_LMSGTABLE ends
        end
