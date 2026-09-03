; sb16_mcode_02.asm -- SB16 object 2 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        or     eax, dword ptr [eax]
loc_2_0010:
        sbb    byte ptr [eax], al
loc_2_0012:
        add    byte ptr es:[ebx], dh
        db  000h
loc_2_0016:
        inc    ecx
        add    byte ptr [ebp], ah
        int3
        int3
loc_2_001C:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 69h], cl
        db  078h, 065h
        jb     SHORT loc_2_0027
loc_2_0027:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [edi + 61h], dl
        db  076h, 065h
        and    byte ptr [ecx + 6eh], cl
        add    byte ptr [ebx + 42h], dl
        xor    dword ptr [esi], esi
        and    byte ptr [edi + 61h], dl
        db  076h, 065h
        and    byte ptr [edi + 75h], cl
        je     SHORT loc_2_0042
loc_2_0042:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 49h], cl
        inc    esp
        dec    ecx
        and    byte ptr [ecx + 6eh], cl
        add    byte ptr [ebx + 42h], dl
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 49h], cl
        inc    esp
        dec    ecx
        and    byte ptr [edi + 75h], cl
        je     SHORT loc_2_005D
loc_2_005D:
        push   ebx
        outsd
        db  075h, 06Eh
        inc    edx
        insb
        popad
        db  073h, 074h
        db  065h, 072h, 020h
        xor    dword ptr [esi], esi
        and    byte ptr [ecx + ebp*2 + 72h], al
        arpl   word ptr gs:[eax + 53h], si
        outsd
        db  075h, 06Eh
        and    byte ptr fs:[edx + esi*2 + 69h], al
        db  076h, 065h
        db  072h, 000h
_LMSGTABLE ends
        end
