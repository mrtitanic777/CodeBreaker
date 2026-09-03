; perf_mcode_02.asm -- PERF object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  00Bh, 000h, 000h, 000h
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        or     byte ptr [eax], al
loc_2_0010:
        adc    dword ptr [eax], eax
loc_2_0012:
        and    eax, dword ptr [eax]
loc_2_0014:
        cmp    byte ptr [eax], al
loc_2_0016:
        jo     SHORT loc_2_0018
loc_2_0018:
        lodsd
        add    byte ptr [eax], al
        db  001h
loc_2_001C:
        or     byte ptr [ecx], al
loc_2_001E:
        adc    dword ptr [ecx], eax
loc_2_0020:
        push   ss
        add    dword ptr [eax], esp
        add    dword ptr [ebx + 65h], ecx
        db  072h, 06Eh
        insb
        add    byte ptr [eax], al
        push   esp
        push   64616572h
        jae    SHORT loc_2_0034
loc_2_0034:
        add    byte ptr [esi + 69h], dl
        db  072h, 074h
        db  075h, 061h
        insb
        and    byte ptr [ebp + 61h], cl
        arpl   word ptr [eax + 69h], bp
        outsb
        jae    NEAR PTR loc_2_0046
loc_2_0046:
        add    byte ptr [eax + 72h], dl
        outsd
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_2_00BE
        db  072h, 020h
        push   ebp
        db  073h, 061h
        and    byte ptr gs:[bx + si], ch
        and    eax, 54000029h
        push   756e2065h
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [eax + 72h], dh
        outsd
        arpl   word ptr [ebp + 73h], sp
        db  073h, 06Fh
        jb     SHORT loc_2_0094
        je     SHORT loc_2_00DE
        jb     SHORT loc_2_00DD
        popad
        db  064h, 073h, 020h
        db  070h, 072h
        jae    NEAR PTR loc_2_00E6
        outsb
        je     SHORT loc_2_00A4
        imul   ebp, dword ptr [esi + 20h], 20656874h
        db  073h, 079h
        db  073h, 074h
        insd
        add    byte ptr cs:[eax], al
loc_2_0094:
        push   esp
        push   756e2065h
        insd
        bound  esp, qword ptr [ebp + 72h]
        and    byte ptr [edi + 66h], ch
        and    byte ptr [esi + 69h], dh
loc_2_00A4:
        jb     SHORT loc_2_011A
        jne    SHORT loc_2_0109
        insb
        and    byte ptr [ebp + 61h], ch
        arpl   word ptr [eax + 69h], bp
        outsb
        db  065h, 073h, 020h
        sub    byte ptr [esi + 4dh], dl
        db  073h, 029h
        and    byte ptr [eax + 72h], dh
        db  065h, 073h, 065h
loc_2_00BE:
        outsb
        db  074h, 020h
        imul   ebp, dword ptr [esi + 20h], 20656874h
        db  073h, 079h
        jae    SHORT loc_2_0140
        insd
        add    byte ptr cs:[eax], al
        push   esp
        push   65702065h
        jb     SHORT loc_2_013C
        outsb
        je     SHORT loc_2_00FD
loc_2_00DD:
        outsd
loc_2_00DE:
        and    byte ptr [eax + 72h], dh
        outsd
        arpl   word ptr [ebp + 73h], sp
loc_2_00E6:
        db  073h, 06Fh
        db  072h, 020h
        db  074h, 069h
        insd
        and    byte ptr gs:[eax + ebp*2 + 61h], dh
        db  074h, 020h
        imul   esi, dword ptr [ebx + 20h], 20746f6eh
        db  073h, 070h
loc_2_00FD:
        outsb
        je     SHORT loc_2_0121
        imul   esp, dword ptr [esp + ebp*2 + 65h], 5420202eh
loc_2_0109:
        push   76207369h
        popad
        insb
        db  075h, 065h
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [ecx + 70h], ah
        db  070h, 072h
loc_2_011A:
        outsd
        db  078h, 069h
        insd
        popad
        db  074h, 065h
loc_2_0121:
        add    byte ptr cs:[eax], al
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        push   esp
        push   64616572h
        jae    SHORT loc_2_0134
loc_2_0134:
        add    byte ptr [esi + 4dh], dl
        jae    SHORT loc_2_0139
loc_2_0139:
        add    byte ptr [ebx + 50h], al
loc_2_013C:
        push   ebp
        push   ebp
        db  073h, 061h
loc_2_0140:
        add    byte ptr gs:[bx + si], al
_LMSGTABLE ends
        end
