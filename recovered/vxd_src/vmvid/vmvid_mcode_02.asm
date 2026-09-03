; vmvid_mcode_02.asm -- VMVID object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        pop    ecx
        add    byte ptr [esi], bh
        int3
        int3
loc_2_0014:
        push   ebp
        outsb
        popad
        bound  ebp, qword ptr [ebp + 20h]
        db  074h, 06Fh
        and    byte ptr [eax + 6ch], dh
        popad
        db  079h, 020h
        db  073h, 06Fh
        db  075h, 06Eh
        and    byte ptr fs:[65687420h], ch
        and    byte ptr [ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 20h], 69736956h
        outsd
        outsb
        and    byte ptr [ecx + 75h], ah
        imul   ebp, dword ptr fs:[edi + 20h], 64726168h
        db  077h, 061h
        db  072h, 065h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 6eh], ch
        and    byte ptr [ebp + 73h], dh
        and    byte ptr gs:[edx + 79h], ah
        and    byte ptr [ecx + 6eh], ah
        outsd
        db  074h, 068h
        db  065h, 072h, 020h
        popad
        db  070h, 070h
        insb
        imul   esp, dword ptr [ebx + 61h], 6e6f6974h
        add    byte ptr cs:[ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 56h], 6f697369h
        outsb
        and    byte ptr [edx + 61h], cl
        db  07Ah, 07Ah
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
