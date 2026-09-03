; azt16_mcode_02.asm -- AZT16 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    edi
        db  000h
loc_2_0010:
        stosd
        add    dh, al
        add    byte ptr [ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        je     SHORT loc_2_008C
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_0043
        db  073h, 06Fh
        db  075h, 06Eh
        and    byte ptr fs:[65687420h], ch
        and    byte ptr [ecx + 7ah], al
        db  074h, 065h
        arpl   word ptr [eax + 20h], bp
        push   ebx
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[ebx + 61h], al
        db  072h, 064h
        and    byte ptr [ecx + 73h], ch
loc_2_0043:
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        jb     NEAR PTR loc_2_0076
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
        jns    SHORT loc_2_0092
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0076:
        and    byte ptr [65687420h], ch
        and    byte ptr [ecx + 7ah], al
        db  074h, 065h
        arpl   word ptr [eax + 20h], bp
        push   ebx
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[ebx + 61h], al
loc_2_008C:
        db  072h, 064h
        and    byte ptr [ebp + 49h], cl
        inc    esp
loc_2_0092:
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
        db  065h, 072h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[ecx + 7ah], al
        db  074h, 065h
        arpl   word ptr [eax + 20h], bp
        inc    esp
        imul   esi, dword ptr [edx + 65h], 53207463h
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
