; mssndsys_mcode_02.asm -- MSSNDSYS object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        push   edx
        db  000h
loc_2_0010:
        mov    cl, 0
        fiadd  dword ptr [eax]
loc_2_0014:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_008C
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_0043
        jae    SHORT loc_2_0094
        db  075h, 06Eh
        and    byte ptr fs:[65687420h], ch
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 6fh], dl
        db  075h, 06Eh
        and    byte ptr fs:[ebx + 79h], dl
        jae    SHORT loc_2_00B5
        insd
loc_2_0043:
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        jb     NEAR PTR loc_2_0079
        popad
        jo     SHORT loc_2_00CC
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 6ch], dh
        popad
        db  079h, 020h
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0079:
        and    byte ptr [65687420h], ch
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 6fh], dl
        db  075h, 06Eh
loc_2_008C:
        and    byte ptr fs:[ebx + 79h], dl
        db  073h, 074h
        insd
loc_2_0094:
        and    byte ptr [ebp + 49h], cl
        inc    esp
        dec    ecx
        and    byte ptr [ebx + 79h], dl
        outsb
        db  074h, 068h
        db  065h, 073h, 069h
        db  07Ah, 065h
        db  072h, 020h
        imul   esi, dword ptr [ebx + 20h], 20657375h
        bound  edi, qword ptr [ecx + 20h]
        popad
        outsb
        outsd
        db  074h, 068h
loc_2_00B5:
        db  065h, 072h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
loc_2_00CC:
        and    byte ptr [ebx + 6fh], dl
        db  075h, 06Eh
        and    byte ptr fs:[ebx + 79h], dl
        db  073h, 074h
        insd
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
