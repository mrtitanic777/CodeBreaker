; vfbackup_mcode_02.asm -- VFBACKUP object 2 segment _LMSGTABLE
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
        inc    esp
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
loc_2_0014:
        push   esi
        inc    esi
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        push   ebp
        push   eax
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
        and    byte ptr cs:[eax + 6ch], dl
        popad
        db  073h, 065h
        and    byte ptr [edx + 75h], dh
        outsb
        and    byte ptr [ebx + 65h], dh
        db  074h, 075h
        db  070h, 020h
        popad
        popad
        imul   ebp, dword ptr [esi + 2eh], 46560000h
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        push   ebp
        push   eax
        and    byte ptr [ebx + 6fh], ah
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_008B
        insb
        outsd
        popad
        and    byte ptr fs:[esi + 46h], dl
        inc    esp
        push   esi
        pop    eax
        inc    esp
        and    byte ptr cs:[eax], ah
        push   eax
        insb
        popad
        db  073h, 065h
        and    byte ptr [edx + 75h], dh
        outsb
        and    byte ptr [ebx + 65h], dh
        db  074h, 075h
        db  070h, 020h
loc_2_008B:
        popad
        popad
        db  069h, 06Eh, 000h, 000h
_LMSGTABLE ends
        end
