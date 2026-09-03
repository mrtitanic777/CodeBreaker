; es1688_mcode_02.asm -- ES1688 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    esp
        db  000h
loc_2_0010:
        wait
        db  000h
loc_2_0012:
        ljmp   55cch:0cc010500h
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 6ch], dh
        popad
        jns    SHORT loc_2_0047
        jae    SHORT loc_2_0098
        db  075h, 06Eh
        and    byte ptr fs:[65687420h], ch
        and    byte ptr [ebp + 53h], al
        push   ebx
        and    byte ptr [ecx + 75h], al
        imul   ebp, dword ptr fs:[edi + 44h], 65766972h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
loc_2_0047:
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        je     SHORT loc_2_00BC
        jb     NEAR PTR loc_2_0077
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
loc_2_0077:
        and    byte ptr [65687420h], ch
        and    byte ptr [ebp + 53h], al
        push   ebx
        and    byte ptr [esi + 4dh], al
        and    byte ptr [ebx + 79h], dl
        outsb
        je     SHORT loc_2_00F2
        jae    NEAR PTR loc_2_00F6
        db  07Ah, 065h
        jb     SHORT loc_2_00B1
        imul   esi, dword ptr [ebx + 20h], 75206e69h
loc_2_0098:
        db  073h, 065h
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        jb     NEAR PTR loc_2_00C6
        popad
        jo     SHORT loc_2_0119
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
loc_2_00B1:
        add    byte ptr cs:[ebp + 6eh], dl
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
loc_2_00BC:
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        dec    ebp
        dec    ecx
loc_2_00C6:
        inc    esp
        dec    ecx
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [65687420h], ch
        and    byte ptr [ebp + 53h], al
        push   ebx
        and    byte ptr [ebp + 50h], cl
        push   ebp
        sub    eax, 20313034h
        imul   esi, dword ptr [ebx + 20h], 75206e69h
        db  073h, 065h
        and    byte ptr [edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
loc_2_00F2:
        db  065h, 072h, 020h
        popad
loc_2_00F6:
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[ebp + 53h], al
        xor    dword ptr [esi], esi
        cmp    byte ptr [eax], bh
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
loc_2_0119:
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
