; vdd_mcode_02.asm -- VDD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        pop    edi
        add    byte ptr [esi + 43000000h], ah
        popad
        outsb
        outsb
        outsd
        je     SHORT loc_2_003B
        jb     SHORT loc_2_0092
        outsb
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [edx + 65h], ah
        arpl   word ptr [ecx + 75h], sp
        db  073h, 065h
        and    byte ptr [edi + 66h], ch
        and    byte ptr [esi + 69h], dh
        outsd
        and    byte ptr [ebp + 76h], ah
loc_2_003B:
        imul   esp, dword ptr [ebx + 65h], 6e6f6320h
        insb
        imul   esp, dword ptr [ebx + 74h], 6f59202eh
        jne    SHORT loc_2_006D
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 075h
        outsb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dl
        db  074h, 075h
        jo     SHORT loc_2_0083
        db  070h, 072h
        outsd
        db  067h, 072h, 061h
        insd
        and    byte ptr [ecx + 67h], ah
loc_2_006D:
        popad
        imul   ebp, dword ptr [esi + 2eh], 69560000h
        outsd
        and    byte ptr [ecx + 6eh], ch
        imul   esi, dword ptr [ecx + ebp*2 + 61h], 617a696ch
loc_2_0083:
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [esi + 61h], ah
        imul   ebp, dword ptr [ebp + 64h], 6f59202eh
loc_2_0092:
        jne    SHORT loc_2_00B4
        outsb
        and    byte ptr fs:[edi + ebp*2 + 20h], dh
        db  072h, 075h
        outsb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], dl
        db  074h, 075h
        db  070h, 020h
        db  070h, 072h
        outsd
        db  067h, 072h, 061h
        insd
        and    byte ptr [ecx + 67h], ah
loc_2_00B4:
        popad
        db  069h, 06Eh, 02Eh, 000h, 000h
_LMSGTABLE ends
        end
