; shell_mcode_03.asm -- SHELL object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  011h, 000h, 000h, 000h
loc_3_0004:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        push   es
        add    byte ptr [edx], cl
        add    byte ptr [esi], cl
        add    byte ptr [edx], dl
        add    byte ptr [esi], dl
        add    byte ptr [edx], bl
        add    byte ptr [25002100h], bl
        add    byte ptr [ecx], ch
        db  000h
loc_3_0024:
        sub    al, 0
loc_3_0026:
        das
        db  000h
loc_3_0028:
        xor    dword ptr [eax], eax
loc_3_002A:
        xor    eax, dword ptr [eax]
loc_3_002C:
        xor    eax, 1c003d00h
        or     eax, 1b0100h
        push   ds
        inc    ecx
        popad
        add    byte ptr [ebx], dl
        push   edx
        jb     SHORT loc_3_003E
loc_3_003E:
        pop    ss
        dec    ecx
        imul   eax, dword ptr [eax], 795915h
        xor    dword ptr [esi + 6eh], ecx
        add    byte ptr [ecx], al
        sbb    eax, dword ptr [eax]
        adc    eax, 31007959h
        dec    esi
        outsb
        add    byte ptr [ebx], dl
        push   edx
        jb     SHORT loc_3_0059
loc_3_0059:
        add    dword ptr [ebx], ebx
        add    byte ptr [ecx + 35000100h], bl
        add    byte ptr [eax], dh
        add    byte ptr [edi + 69h], dl
        outsb
        outsd
        db  077h, 073h
        db  000h
_LMSGTABLE ends
        end
