; parity_mcode_02.asm -- PARITY object 2 segment _LMSGTABLE
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
        pop    ds
        db  000h
loc_2_0010:
        dec    ecx
        add    byte ptr [edi], dl
loc_2_0014:
        dec    ebp
        insd
        outsd
        db  072h, 079h
        and    byte ptr [eax + 61h], dh
        db  072h, 069h
        db  074h, 079h
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        db  072h, 020h
        db  064h, 065h, 074h, 065h
        arpl   word ptr [ebp + 64h], si
        add    byte ptr cs:[eax], al
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 6168436fh
        outsb
        outsb
        insb
        and    byte ptr [edx + 75h], ah
        db  073h, 020h
        db  074h, 069h
        insd
        outsd
        db  075h, 074h
        and    byte ptr [ebp + 72h], ah
        db  072h, 06Fh
        db  072h, 020h
        db  064h, 065h, 074h, 065h
        arpl   word ptr [ebp + 64h], si
        add    byte ptr cs:[eax], al
        push   ebx
        db  079h, 073h
        db  074h, 065h
        insd
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        db  072h, 000h
        db  000h
_LMSGTABLE ends
        end
