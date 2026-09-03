; vpowerd_mcode_02.asm -- VPOWERD object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  003h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        sbb    al, byte ptr [eax]
loc_2_0010:
        mov    dh, 0
        je     SHORT loc_2_0015
loc_2_0014:
        inc    ecx
loc_2_0015:
        db  064h, 076h, 061h
        outsb
        arpl   word ptr [ebp + 64h], sp
        and    byte ptr [eax + 6fh], dl
        ja     SHORT loc_2_0086
        jb     SHORT loc_2_0043
        dec    ebp
        popad
        outsb
        popad
        insd
        outsb
        je     SHORT loc_2_002E
loc_2_002E:
        pop    ecx
        outsd
        db  075h, 072h
        and    byte ptr [ebx + 6fh], ah
        insd
        db  070h, 075h
        je     SHORT loc_2_009F
        db  072h, 020h
        arpl   word ptr [ecx + 6eh], sp
        outsb
        outsd
        je     SHORT loc_2_0063
loc_2_0043:
        bound  esp, qword ptr [ebp + 20h]
        jae    SHORT loc_2_00BD
        db  073h, 070h
        outsb
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [eax + ebp*2 + 69h], dh
        db  073h, 020h
        db  074h, 069h
        insd
        or     eax, 72702041h
        outsd
loc_2_0063:
        db  067h, 072h, 061h
        insd
        and    byte ptr [edi + 72h], ch
        and    byte ptr [edx + esi*2 + 69h], ah
        db  076h, 065h
        jb     SHORT loc_2_0092
        db  079h, 06Fh
        db  075h, 020h
        push   20657661h
        jb     SHORT loc_2_00F2
        outsb
        outsb
        imul   ebp, dword ptr [esi + 67h], 79616d20h
loc_2_0086:
        and    byte ptr [edx + 65h], ah
        and    byte ptr [eax + 72h], dh
        jbe    NEAR PTR loc_2_00F4
        outsb
        db  074h, 069h
loc_2_0092:
        outsb
        and    byte ptr [si + 68h], dh
        imul   esi, dword ptr [ebx + 20h], 69746361h
        outsd
loc_2_009F:
        outsb
        or     eax, 736f6c43h
        and    byte ptr gs:[ecx + 6ch], ah
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  067h, 072h, 061h
        insd
        db  073h, 02Ch
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
loc_2_00BD:
        outsb
        and    byte ptr [edx + esi*2 + 79h], dh
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 2eh], 756f5900h
        db  072h, 020h
        arpl   word ptr [edi + 6dh], bp
        jo     SHORT loc_2_0149
        db  074h, 065h
        db  072h, 020h
        arpl   word ptr [ecx + 6eh], sp
        outsb
        outsd
        db  074h, 020h
        bound  esp, qword ptr [ebp + 20h]
        db  073h, 075h
        jae    SHORT loc_2_0156
        outsb
        and    byte ptr fs:[ecx + 74h], ah
        and    byte ptr [eax + ebp*2 + 69h], dh
loc_2_00F2:
        db  073h, 020h
loc_2_00F4:
        db  074h, 069h
        insd
        or     eax, 736f6c43h
        and    byte ptr gs:[ecx + 6ch], ah
        insb
        and    byte ptr [eax + 72h], dh
        outsd
        db  067h, 072h, 061h
        insd
        db  073h, 02Ch
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        outsb
        and    byte ptr [edx + esi*2 + 79h], dh
        and    byte ptr [ecx + 67h], ah
        popad
        imul   ebp, dword ptr [esi + 2eh], 2066490dh
        db  079h, 06Fh
        jne    SHORT loc_2_0149
        arpl   word ptr [edi + 6eh], bp
        db  074h, 069h
        outsb
        db  075h, 065h
        and    byte ptr [edi + ebp*2 + 20h], dh
        db  073h, 065h
        and    byte ptr gs:[eax + ebp*2 + 69h], dh
        jae    SHORT loc_2_015E
        insd
        db  065h, 073h, 073h
        popad
        sub    al, 0dh
        db  073h, 065h
loc_2_0149:
        and    byte ptr gs:[ecx + 6fh], bh
        db  075h, 072h
        and    byte ptr [eax + 61h], ch
        db  072h, 064h
        db  077h, 061h
loc_2_0156:
        db  072h, 065h
        and    byte ptr [edi + ebp*2 + 63h], ah
        db  075h, 06Dh
loc_2_015E:
        outsb
        db  074h, 061h
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [edi + 72h], ch
        and    byte ptr [ebx + 6fh], ah
        outsb
        db  074h, 061h
        arpl   word ptr [eax + 79h], si
        outsd
        db  075h, 072h
        or     eax, 706d6f63h
        db  075h, 074h
        db  065h, 072h, 020h
        db  076h, 065h
        outsb
        outsd
        db  072h, 02Eh
        db  000h
_LMSGTABLE ends
        end
