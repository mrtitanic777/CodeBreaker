; msopl_mcode_02.asm -- MSOPL object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 049h, 000h
loc_2_0010:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_2_0041
        inc    esi
        dec    ebp
        and    byte ptr [ebx + 79h], dh
        outsb
        db  074h, 068h
        db  065h, 073h, 069h
        db  073h, 020h
        db  064h, 065h, 076h, 069h
        arpl   word ptr [ebp + 20h], sp
        sub    eax, 20746920h
        imul   ebp, dword ptr [esi + 20h], 20657375h
loc_2_0041:
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
        db  02Eh, 000h
_LMSGTABLE ends
        end
