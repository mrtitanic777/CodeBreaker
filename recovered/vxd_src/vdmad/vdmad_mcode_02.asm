; vdmad_mcode_02.asm -- VDMAD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_2_0000:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jp     SHORT loc_2_0010
loc_2_0010:
        push   esp
        push   4d442065h
        inc    ecx
        and    byte ptr [edx + 75h], ah
        jb     NEAR PTR loc_2_003F
        imul   esi, dword ptr [ebx + 20h], 206f6f74h
        db  073h, 06Dh
        popad
        insb
        insb
        or     eax, 7465530ah
        and    byte ptr [edx], ah
        inc    esp
        dec    ebp
        inc    ecx
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        dec    ecx
        pop    edx
loc_2_003F:
        inc    ebp
        cmp    eax, 20227525h
        imul   ebp, dword ptr [esi + 20h], 54535953h
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 33h], bl
        cmp    byte ptr [esi], dh
        inc    ebp
        outsb
        push   6573205dh
        arpl   word ptr [ecx + ebp*2 + 6fh], si
        outsb
        or     eax, 610a0d0ah
        outsb
        and    byte ptr fs:[edx + 65h], dh
        db  073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        db  074h, 065h
        db  072h, 02Eh
        add    byte ptr [eax], al
_LMSGTABLE ends
        end
