; vkd_mcode_02.asm -- VKD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        arpl   word ptr [eax], ax
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
loc_2_0014:
        push   esp
        push   69762065h
        db  072h, 074h
        jne    SHORT loc_2_007F
        insb
        and    byte ptr [ebx + 65h], ch
        db  079h, 062h
        outsd
        popad
        db  072h, 064h
        and    byte ptr [edx + esi*2 + 69h], ah
        jbe    SHORT loc_2_0093
        jb     SHORT loc_2_0050
        popaw
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        db  076h, 069h
        jb     SHORT loc_2_00B2
        db  075h, 061h
        insb
        imul   edi, dword ptr [edx + 65h], 72616820h
        db  064h, 077h, 061h
        jb     SHORT loc_2_00B2
        and    byte ptr [ecx + 6eh], ch
loc_2_0050:
        je     SHORT loc_2_00B7
        db  072h, 072h
        db  075h, 070h
        db  074h, 00Dh
        or     ch, byte ptr [esi + 75h]
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [ecx], dh
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        db  074h, 068h
        and    byte ptr gs:[ebx + 65h], ch
        db  079h, 062h
        outsd
        popad
        db  072h, 064h
        or     eax, 5400000ah
        push   69762065h
        db  072h, 074h
loc_2_007F:
        db  075h, 061h
        insb
        and    byte ptr [ebx + 65h], ch
        db  079h, 062h
        outsd
        popad
        jb     SHORT loc_2_00EF
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        db  072h, 020h
loc_2_0093:
        popaw
        imul   ebp, dword ptr [ebp + 64h], 206f7420h
        imul   ebp, dword ptr [esi + 73h], 6c6c6174h
        and    byte ptr [ecx + 74h], ch
        jae    SHORT loc_2_00C9
        push   206b6f6fh
        outsw
        db  072h, 020h
loc_2_00B2:
        db  073h, 06Fh
        db  066h, 074h, 077h
loc_2_00B7:
        popad
        db  072h, 065h
        and    byte ptr [ecx + 6eh], ch
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        db  074h, 00Dh
        or     ch, byte ptr [esi + 75h]
        insd
loc_2_00C9:
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [ecx], dh
        push   726f6620h
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 65h], ch
        db  079h, 062h
        outsd
        popad
        db  072h, 064h
        or     eax, 500a0d0ah
        insb
        popad
        db  073h, 065h
        and    byte ptr [edx + 75h], dh
loc_2_00EF:
        outsb
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        and    byte ptr [ebx + 65h], dl
        db  074h, 075h
        db  070h, 020h
        db  070h, 072h
        outsd
        db  067h, 072h, 061h
        insd
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 2eh], 0a0dh
_LMSGTABLE ends
        end
