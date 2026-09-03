; enable_mcode_02.asm -- ENABLE object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  00Ah, 000h, 000h, 000h
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        adc    al, 0
        inc    ebx
        add    byte ptr [ebx], ah
loc_2_0014:
        xchg   ebx, eax
        db  000h, 0B9h, 000h
loc_2_0018:
        in     eax, 0
        db  00Dh, 001h, 033h, 001h
loc_2_001E:
        pop    edi
        add    dword ptr [edi + 41000001h], eax
        arpl   word ptr [ebx + 65h], sp
        db  073h, 073h
        imul   esp, dword ptr [edx + 69h], 7974696ch
        and    byte ptr [ebp + 72h], al
        db  072h, 06Fh
        jb     SHORT loc_2_0038
loc_2_0038:
        dec    eax
        imul   esp, dword ptr [edi + 68h], 746e6f43h
        jb     SHORT loc_2_00A3
        db  073h, 074h
        and    byte ptr [ebx + 61h], ah
        outsb
        and    byte ptr [edi + 6eh], ch
        insb
        db  079h, 020h
        bound  esp, qword ptr [ebp + 20h]
        db  073h, 065h
        db  074h, 020h
        outsw
        db  072h, 020h
        je     SHORT loc_2_00C3
        and    byte ptr gs:[ebx + 79h], dh
        jae    SHORT loc_2_00D5
        insd
        and    byte ptr [esi + 4dh], dl
        add    byte ptr [ebx + 6fh], al
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0091
        insb
        outsd
        arpl   word ptr [ecx + 74h], sp
        and    byte ptr gs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ecx + 63h], al
        arpl   word ptr [ebp + 73h], sp
        jae    SHORT loc_2_00A3
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [ebx + 6fh], al
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_00B1
loc_2_0091:
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 65687420h
        and    byte ptr [ebp + 69h], cl
        outsb
loc_2_00A3:
        imul   esp, dword ptr [eax], 2c447856h
        and    byte ptr [edi + ebp*2 + 61h], ch
        and    byte ptr fs:[ecx + 62h], ah
loc_2_00B1:
        outsd
        jb     SHORT loc_2_0128
        add    byte ptr fs:[ebx + 6fh], al
        jne    SHORT loc_2_0127
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_00E1
        insb
        outsd
loc_2_00C3:
        popad
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 6eh], ah
        popad
        bound  ebp, qword ptr [ebp + 32h]
        push   esi
        pop    eax
        inc    esp
loc_2_00D5:
        and    byte ptr [ebp + 6fh], ch
        jne    NEAR PTR loc_2_0147
        add    byte ptr gs:[ebx + 6fh], al
        jne    SHORT loc_2_014D
loc_2_00E1:
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0107
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 65687420h
        and    byte ptr [ebp + 6eh], ah
        popad
        bound  ebp, qword ptr [ebp + 32h]
        push   esi
        pop    eax
        inc    esp
        and    byte ptr [ebp + 6fh], ch
        jne    NEAR PTR loc_2_0173
loc_2_0107:
        add    byte ptr gs:[ebx + 6fh], al
        jne    SHORT loc_2_0179
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0133
        db  075h, 06Eh
        insb
        outsd
        popad
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 6eh], ah
        popad
        bound  ebp, qword ptr [ebp + 32h]
        push   esi
loc_2_0127:
        pop    eax
loc_2_0128:
        inc    esp
        and    byte ptr [ebp + 6fh], ch
        jne    NEAR PTR loc_2_019B
        add    byte ptr gs:[ebx + 6fh], al
loc_2_0133:
        jne    SHORT loc_2_01A1
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_015B
        insb
        outsd
        popad
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 6eh], ah
        popad
loc_2_0147:
        bound  ebp, qword ptr [ebp + 34h]
        push   esi
loc_2_014D:
        pop    eax
        inc    esp
        and    byte ptr [ebp + 6fh], ch
        db  064h, 075h, 06Ch
        add    byte ptr gs:[ebx + 6fh], al
        db  075h, 06Ch
loc_2_015B:
        and    byte ptr fs:[esi + 6fh], ch
        je     SHORT loc_2_0181
        imul   ebp, dword ptr [esi + 69h], 6c616974h
        imul   edi, dword ptr [edx + 65h], 65687420h
        and    byte ptr [ebp + 6eh], ah
        popad
loc_2_0173:
        bound  ebp, qword ptr [ebp + 34h]
        push   esi
loc_2_0179:
        pop    eax
        inc    esp
        and    byte ptr [ebp + 6fh], ch
        db  064h, 075h, 06Ch
loc_2_0181:
        add    byte ptr gs:[ebx + 6fh], al
        db  075h, 06Ch
        and    byte ptr fs:[esi + 6fh], ch
        db  074h, 020h
        db  075h, 06Eh
        insb
        outsd
        popad
        and    byte ptr fs:[eax + ebp*2 + 65h], dh
        and    byte ptr [ebp + 6eh], ah
        popad
loc_2_019B:
        bound  ebp, qword ptr [ebp + 34h]
        push   esi
loc_2_01A1:
        pop    eax
        inc    esp
        and    byte ptr [ebp + 6fh], ch
        db  064h, 075h, 06Ch
        db  065h, 000h
_LMSGTABLE ends
        end
