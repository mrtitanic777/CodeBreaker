; msmpu401_mcode_02.asm -- MSMPU401 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 04Bh, 000h
loc_2_0010:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [65687420h], ch
        and    byte ptr [ebp + 50h], cl
        push   ebp
        sub    eax, 20313034h
        imul   esi, dword ptr [ebx + 20h], 75206e69h
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
        db  02Eh, 000h
_LMSGTABLE ends
        end
