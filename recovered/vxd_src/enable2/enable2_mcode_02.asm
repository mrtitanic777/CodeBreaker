; enable2_mcode_02.asm -- ENABLE2 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        or     al, 0
        sbb    eax, 4d003600h
        add    byte ptr [ebx], ch
loc_2_0018:
        inc    ebp
        dec    esi
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        xor    ebp, dword ptr [esi]
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [eax + 45h], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        inc    esi
        dec    esp
        inc    ecx
        push   ebx
        dec    eax
        dec    ecx
        push   esp
        inc    ebp
        dec    ebp
        add    byte ptr [eax + 45h], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        inc    ebx
        dec    esp
        inc    ebp
        inc    ecx
        push   edx
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ebx
        dec    edi
        dec    esi
        push   esp
        push   edx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax + 45h], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        push   ebx
        inc    ebp
        push   esp
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ebx
        dec    edi
        dec    esi
        push   esp
        push   edx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax + 45h], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        inc    ebx
        push   edx
        inc    ebp
        inc    ecx
        push   esp
        inc    ebp
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    esp
        inc    ecx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        db  000h
_LMSGTABLE ends
        end
