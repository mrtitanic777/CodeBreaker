; mssblst_mcode_02.asm -- MSSBLST object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    ebx
        add    byte ptr [esi + 5500cb00h], ah
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 6ch], dh
        popad
        db  079h, 020h
        jae    SHORT loc_2_0094
        db  075h, 06Eh
        and    byte ptr fs:[65687420h], ch
        and    byte ptr [ebx + 6fh], dl
        jne    SHORT loc_2_00A1
        and    byte ptr fs:[edx + 6ch], al
        popad
        jae    SHORT loc_2_00AE
        jb     NEAR PTR loc_2_005D
        imul   esi, dword ptr [ebx + 20h], 75206e69h
        db  073h, 065h
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        jb     NEAR PTR loc_2_0072
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
loc_2_005D:
        add    byte ptr cs:[ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_008E
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0072:
        and    byte ptr [65687420h], ch
        and    byte ptr [ebx + 6fh], dl
        db  075h, 06Eh
        and    byte ptr fs:[edx + 6ch], al
        popad
        db  073h, 074h
        jb     NEAR PTR loc_2_00A7
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
        and    byte ptr [ebx + 79h], dl
loc_2_008E:
        outsb
        db  074h, 068h
        db  065h, 073h, 069h
loc_2_0094:
        db  07Ah, 065h
        jb     SHORT loc_2_00B8
        imul   esi, dword ptr [ebx + 20h], 75206e69h
        db  073h, 065h
loc_2_00A1:
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
loc_2_00A7:
        outsd
        db  074h, 068h
        db  065h, 072h, 020h
        popad
loc_2_00AE:
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
loc_2_00B8:
        add    byte ptr cs:[ebx + 6fh], dl
        db  075h, 06Eh
        inc    edx
        insb
        popad
        db  073h, 074h
        db  065h, 072h, 020h
        push   eax
        db  072h, 06Fh
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
