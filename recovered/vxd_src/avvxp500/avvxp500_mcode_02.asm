; avvxp500_mcode_02.asm -- AVVXP500 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 04Dh, 000h
loc_2_0010:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [ecx + 63h], ah
        arpl   word ptr [ebp + 73h], sp
        db  073h, 020h
        arpl   word ptr [ecx + 70h], sp
        db  074h, 075h
        db  072h, 065h
        and    byte ptr [eax + 6fh], dh
        db  072h, 074h
        and    byte ptr [65687420h], ch
        and    byte ptr [esi + 78h], dl
        push   eax
        xor    eax, 69203030h
        db  073h, 020h
        imul   ebp, dword ptr [esi + 20h], 20657375h
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
