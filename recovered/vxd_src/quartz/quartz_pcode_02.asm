; quartz_pcode_02.asm -- QUARTZ object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        xor    eax, eax
        clc
        ret
loc_2_0004:
        jecxz  loc_2_0016
        dec    ecx
        je     SHORT loc_2_0039
        dec    ecx
        je     NEAR PTR loc_2_00A6
        mov    eax, 1
        ret
loc_2_0016:
        push   edi
        push   esi
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_2_0034
        mov    edi, dword ptr [esi + 20h]
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], 100h
        or     edi, edi
        je     SHORT loc_2_0034
        mov    dword ptr [edi], 4
loc_2_0034:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_2_0039:
        push   edi
        push   esi
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_2_00A1
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [edi], 4
        mov    eax, dword ptr [esi + 10h]
        mov    edi, dword ptr [esi + 10h]
        mov    eax, dword ptr [edi]
        mov    edi, eax
        add    eax, eax
        push   0
        push   eax
        push   80000400h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        mov    esi, dword ptr [esi + 18h]
        mov    dword ptr [esi], eax
        cmp    eax, -1
        je     SHORT loc_2_00A1
        shr    eax, 0ch
        mov    esi, eax
        push   60000h
        push   0
        push   1
        push   edi
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        db  0CDh, 020h, 057h, 001h, 001h, 000h
        mov    ebx, esi
        add    ebx, edi
        push   edi
        push   ebx
        push   eax
        push   esi
        db  0CDh, 020h, 025h, 001h, 001h, 000h
        add    esp, 10h
loc_2_00A1:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
loc_2_00A6:
        push   edi
        push   esi
        cmp    dword ptr [esi + 14h], 4
        jb     SHORT loc_2_00C8
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 10h]
        mov    eax, dword ptr [eax]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_2_00C8:
        pop    esi
        pop    edi
        xor    eax, eax
        ret
_PTEXT ends
        end
