; es488_mcode_02.asm -- ES488 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    esp
        db  000h
loc_2_0010:
        wait
        add    byte ptr [ebp + 616e5500h], dh
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_0043
        jae    SHORT loc_2_0094
        db  075h, 06Eh
        and    byte ptr fs:[65687420h], ch
        and    byte ptr [ebp + 53h], al
        push   ebx
        and    byte ptr [ecx + 75h], al
        imul   ebp, dword ptr fs:[edi + 44h], 65766972h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
loc_2_0043:
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        jb     NEAR PTR loc_2_0073
        popad
        db  070h, 070h
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
loc_2_0073:
        and    byte ptr [65687420h], ch
        and    byte ptr [ebp + 53h], al
        push   ebx
        and    byte ptr [esi + 4dh], al
        and    byte ptr [ebx + 79h], dl
        outsb
        db  074h, 068h
        db  065h, 073h, 069h
        db  07Ah, 065h
        jb     SHORT loc_2_00AD
        imul   esi, dword ptr [ebx + 20h], 75206e69h
loc_2_0094:
        db  073h, 065h
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        db  065h, 072h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
loc_2_00AD:
        add    byte ptr cs:[ebp + 53h], al
        xor    al, 38h
        cmp    byte ptr [eax], ah
        inc    esp
        imul   esi, dword ptr [edx + 65h], 53207463h
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
