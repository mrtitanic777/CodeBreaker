; vpicd_mcode_03.asm -- VPICD object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  001h, 000h, 000h, 000h
loc_3_0004:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
loc_3_0010:
        inc    ebx
        outsd
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_3_003A
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 65687420h
        and    byte ptr [ecx + 6eh], ch
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        db  074h, 020h
        arpl   word ptr [edi + 6eh], bp
        db  074h, 072h
        outsd
        insb
loc_3_003A:
        insb
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
_LMSGTABLE ends
        end
