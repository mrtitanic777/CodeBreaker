; vpasd_mcode_02.asm -- VPASD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        jg     SHORT loc_2_0010
loc_2_0010:
        mov    dword ptr [54cccc00h], eax
        push   72502065h
        outsd
        and    byte ptr [ecx + 75h], al
        imul   ebp, dword ptr fs:[edi + 20h], 63657053h
        db  074h, 072h
        db  075h, 06Dh
        and    byte ptr [eax + 61h], ch
        db  072h, 064h
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6ch], ah
        db  072h, 065h
        popad
        db  064h, 079h, 020h
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
        and    byte ptr cs:[ecx + 75h], al
        imul   ebp, dword ptr fs:[edi + 20h], 70707573h
        outsd
        db  072h, 074h
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [esi + 6fh], ch
        je     SHORT loc_2_0095
        bound  esp, qword ptr [ebp + 20h]
        popad
        db  076h, 061h
        imul   ebp, dword ptr [ecx + 62h], 7420656ch
        outsd
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        db  070h, 072h
        outsd
        db  067h, 072h, 061h
        insd
        add    byte ptr cs:[ebp + 65h], cl
loc_2_0095:
        imul   esp, dword ptr fs:[ecx + 56h], 6f697369h
        outsb
        and    byte ptr [eax + 41h], dl
        push   ebx
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
