; es688_pcode_03.asm -- ES688 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        xor    esi, esi
        db  087h, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_004A
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        push   ebx
loc_3_002F:
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     SHORT loc_3_003E
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_3_003E:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_3_002F
        pop    ebx
loc_3_004A:
        clc
        ret
sub_3_004C:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_005D:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_3_00BD
        mov    ebx, dword ptr [edi + 2ch]
        or     ebx, ebx
        je     SHORT loc_3_00BD
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_00BD
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0078:
        jb     SHORT loc_3_00BD
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_007F:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_3_00AB
        mov    edi, esi
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        mov    word ptr [edi + 20h], dx
        mov    dword ptr [edi + 18h], ecx
        mov    dword ptr [edi + 1ch], esi
        mov    esi, edi
        test   word ptr [esi + 20h], 1
        jne    SHORT loc_3_00AB
        db  0CDh, 020h, 007h, 000h, 004h, 000h
loc_3_00AB:
        pop    edi
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00B5:
        pop    edx
        pop    ecx
        or     word ptr [edi + 4], 100h
loc_3_00BD:
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_3_00C3:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00D4:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_3_014B
        push   edx
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00E5:
        pop    edx
        jb     SHORT loc_3_00ED
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00ED:
        mov    ebx, dword ptr [edi + 2ch]
        or     ebx, ebx
        je     SHORT loc_3_014B
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_3_014B
        test   word ptr [edi + 4], 100h
        je     SHORT loc_3_014B
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0109:
        jb     SHORT loc_3_014B
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0110:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_3_0139
        mov    edi, esi
        mov    dx, word ptr [edi + 20h]
        mov    ecx, dword ptr [edi + 18h]
        mov    esi, dword ptr [edi + 1ch]
        db  0CDh, 020h, 005h, 000h, 004h, 000h
        mov    esi, edi
        test   dl, 1
        jne    SHORT loc_3_0139
        db  0CDh, 020h, 008h, 000h, 004h, 000h
loc_3_0139:
        pop    edi
        movzx  edx, word ptr [edi + 6]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0143:
        pop    edx
        pop    ecx
        and    word ptr [edi + 4], 0feffh
loc_3_014B:
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        pop    ebp
        ret
_PTEXT ends
        end
