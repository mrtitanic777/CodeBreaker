; vpd_pcode_03.asm -- VPD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   ebp
        mov    ebp, esp
        push   3
        pop    ecx
        db  0BAh, 0C0h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
loc_3_000E:
        cmp    dword ptr [edx + 14h], eax
        je     SHORT loc_3_001A
        lea    edx, [edx + 40h]
        loop   loc_3_000E
        xor    edx, edx
loc_3_001A:
        mov    eax, edx
        leave
        ret
loc_3_001E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0ch]
        test   esi, esi
        je     SHORT loc_3_006C
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [esi + 2ch], eax
        jne    SHORT loc_3_005E
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_3_004F
        push   0
        push   dword ptr [esi + 28h]
        call   dword ptr [esi + 24h]
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_3_0072
        mov    eax, dword ptr [ebp + 10h]
loc_3_004F:
        xchg   dword ptr [esi + 24h], eax
        xchg   dword ptr [esi + 2ch], eax
        mov    eax, dword ptr [esi + 28h]
        xchg   dword ptr [esi + 30h], eax
        mov    dword ptr [esi + 28h], eax
loc_3_005E:
        cmp    ebx, dword ptr [esi + 1ch]
        je     SHORT loc_3_0070
        xor    al, al
        db  0E8h, 0CEh, 002h, 0FDh, 0FFh
        jae    SHORT loc_3_0070
loc_3_006C:
        xor    eax, eax
        jmp    SHORT loc_3_0072
loc_3_0070:
        or     al, 1
loc_3_0072:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0077:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [esp + 14h]
        test   esi, esi
        je     SHORT loc_3_00F6
        test   byte ptr [esi + 20h], 8
        je     SHORT loc_3_0097
        mov    eax, dword ptr [esp + 14h]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    dword ptr [esi + 1ch], ebx
        je     SHORT loc_3_00F8
loc_3_0097:
        test   byte ptr [esi + 20h], 80h
        jne    SHORT loc_3_00A2
        db  0E8h, 0B9h, 002h, 0FDh, 0FFh
loc_3_00A2:
        test   byte ptr [esi + 20h], 4
        jne    SHORT loc_3_00F6
        mov    ecx, dword ptr [esi + 24h]
        jecxz  loc_3_00D3
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_3_00F6
        push   0
        push   dword ptr [esi + 28h]
        call   ecx
        pop    ecx
        pop    ecx
        test   eax, eax
        je     SHORT loc_3_00F8
        mov    ecx, dword ptr [esi + 24h]
        jecxz  loc_3_00D3
        mov    edx, dword ptr [esi + 28h]
        mov    dword ptr [esi + 2ch], ecx
        mov    dword ptr [esi + 30h], edx
        or     byte ptr [esi + 20h], 4
loc_3_00D3:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        xor    al, al
        db  0E8h, 058h, 002h, 0FDh, 0FFh
        jb     SHORT loc_3_00F6
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 24h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 28h], eax
        mov    eax, dword ptr [esp + 14h]
        jmp    SHORT loc_3_00F8
loc_3_00F6:
        xor    eax, eax
loc_3_00F8:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_3_00FC:
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 2ch], eax
        je     SHORT loc_3_0147
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        cmp    dword ptr [esi + 1ch], ebx
        jne    SHORT loc_3_011C
        db  0E8h, 023h, 005h, 0FDh, 0FFh
loc_3_011C:
        mov    dword ptr [esi + 24h], 0
        btr    dword ptr [esi + 20h], 2
        jae    SHORT loc_3_014B
        push   0
        push   dword ptr [esi + 30h]
        push   dword ptr [esi + 2ch]
        push   esi
        push   1
        call   sub_3_0077
        add    esp, 14h
        push   1
        push   dword ptr [esi + 28h]
        call   dword ptr [esi + 24h]
        pop    ecx
        pop    ecx
loc_3_0147:
        and    byte ptr [esi + 20h], 0fbh
loc_3_014B:
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_0156
        db  0E8h, 024h, 002h, 0FDh, 0FFh
loc_3_0156:
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_3_015A:
        db  0B8h, 061h, 001h, 000h, 000h
        clc
        ret
loc_3_0161:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 4
        ja     SHORT loc_3_0171
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_3_0171:
        xor    eax, eax
        ret
_PTEXT ends
        end
