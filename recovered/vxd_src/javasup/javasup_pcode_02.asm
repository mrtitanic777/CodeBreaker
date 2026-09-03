; javasup_pcode_02.asm -- JAVASUP object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        push   edi
        popad
        imul   esi, dword ptr [esi + eax*2 + 6fh], 6c754d72h
        db  074h, 069h
        db  070h, 06Ch
        dec    edi
        bound  ebp, qword ptr [edx + 65h]
        arpl   word ptr [ebx + esi*2 + 45h], si
        js     SHORT loc_2_0019
loc_2_0019:
        call   sub_2_0115
        or     eax, eax
        je     SHORT loc_2_0085
        cmp    dword ptr [eax + 18h], 0e904c483h
        jne    SHORT loc_2_0085
        mov    ebx, dword ptr [eax + 1ch]
        lea    eax, [ebx + eax + 20h]
        cmp    dword ptr [eax], 8bc03355h
        jne    SHORT loc_2_0085
        cmp    dword ptr [eax + 10h], 570c5d8bh
        jne    SHORT loc_2_0087
        mov    edi, 0f6h
loc_2_0048:
        add    edi, eax
        cmp    dword ptr [edi], 1f47d81h
        jne    SHORT loc_2_0085
        cmp    dword ptr [edi + 4], 0f400000h
        jne    SHORT loc_2_0085
        call   sub_2_0097
        jb     SHORT loc_2_0085
        mov    esi, edi
        add    edi, 8
        and    edi, 0fffff000h
        cmp    edi, esi
        jbe    SHORT loc_2_0078
        call   sub_2_0097
        jb     SHORT loc_2_0085
loc_2_0078:
        mov    dword ptr [esi], 40f77d80h
        mov    dword ptr [esi + 4], 0f0c5d8bh
loc_2_0085:
        clc
        ret
loc_2_0087:
        cmp    dword ptr [eax + 10h], 57f44589h
        mov    edi, 0fdh
        je     SHORT loc_2_0048
        jmp    SHORT loc_2_0085
sub_2_0097:
        pushad
        mov    ebp, 1000h
        and    edi, 0fffff000h
        xor    eax, eax
        push   20000000h
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        db  074h, 052h
        xchg   esi, eax
        push   ebp
        push   edi
        push   esi
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
        shr    edi, 0ch
        push   20000000h
        push   1
        push   edi
        db  0CDh, 020h, 01Fh, 001h, 001h, 000h
        add    esp, 0ch
        push   20040000h
        push   0
        push   2
        push   1
        push   edi
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        shl    edi, 0ch
        push   ebp
        push   esi
        push   edi
        db  0CDh, 020h, 07Dh, 001h, 001h, 000h
        add    esp, 0ch
        push   0
        push   esi
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        or     al, 0f9h
        popad
        ret
sub_2_0115:
        sub    esp, 8
        mov    eax, 0bff70000h
        cmp    word ptr [eax], 5a4dh
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    NEAR PTR loc_2_01BC
        mov    eax, 0bff7003ch
        mov    eax, dword ptr [eax]
        sub    eax, 40090000h
        cmp    dword ptr [eax], 4550h
        jne    SHORT loc_2_01BC
        mov    eax, dword ptr [eax + 78h]
        mov    ecx, dword ptr [eax - 4008ffe0h]
        mov    ebp, dword ptr [eax - 4008ffe8h]
        sub    eax, 40090000h
        sub    ecx, 40090000h
        dec    ebp
        mov    dword ptr [esp + 14h], ecx
        xor    ecx, ecx
loc_2_0161:
        lea    edx, [ebp + ecx]
        mov    esi, dword ptr [esp + 14h]
        shr    edx, 1
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + edx*4]
        sub    esi, 40090000h
loc_2_0179:
        mov    bl, byte ptr [esi]
        mov    byte ptr [esp + 10h], bl
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_2_01A5
        or     byte ptr [esp + 10h], bl
        je     SHORT loc_2_01A1
        mov    bl, byte ptr [esi + 1]
        mov    byte ptr [esp + 10h], bl
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_2_01A5
        add    esi, 2
        add    edi, 2
        or     byte ptr [esp + 10h], bl
        jne    SHORT loc_2_0179
loc_2_01A1:
        xor    esi, esi
        jmp    SHORT loc_2_01AA
loc_2_01A5:
        sbb    esi, esi
        sbb    esi, -1
loc_2_01AA:
        test   esi, esi
        jle    SHORT loc_2_01B3
        lea    ebp, [edx - 1]
        jmp    SHORT loc_2_01B8
loc_2_01B3:
        jge    SHORT loc_2_01C6
        lea    ecx, [edx + 1]
loc_2_01B8:
        cmp    ecx, ebp
        jbe    SHORT loc_2_0161
loc_2_01BC:
        xor    eax, eax
loc_2_01BE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_2_01C6:
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 1ch]
        movzx  edx, word ptr [ecx + edx*2 - 40090000h]
        mov    eax, dword ptr [eax + edx*4 - 40090000h]
        sub    eax, 40090000h
        jmp    SHORT loc_2_01BE
_PTEXT ends
        end
