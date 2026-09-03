; vcond_pcode_09.asm -- VCOND object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_9_0000:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   edi
        mov    edi, dword ptr [esp + 8]
        je     SHORT loc_9_0016
        mov    word ptr [edi + 1ah], 11ch
        jmp    SHORT loc_9_0046
loc_9_0016:
        db  0C7h, 047h, 010h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 1ch], 50495045h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + 20h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_003B:
        mov    word ptr [edi + 1ah], 0
        db  0A3h, 000h, 000h, 000h, 000h
loc_9_0046:
        movsx  eax, word ptr [edi + 1ah]
        pop    edi
        ret
loc_9_004C:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0052:
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        jne    SHORT loc_9_0064
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 1ah], 1
        jmp    SHORT loc_9_009E
loc_9_0064:
        mov    esi, dword ptr [esp + 8]
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        push   dword ptr [ecx + 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esi + 10h]
        push   dword ptr [ecx]
        db  0C7h, 040h, 004h, 000h, 000h, 000h, 000h
        db  0C7h, 040h, 008h, 000h, 000h, 000h, 000h
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0098:
        mov    word ptr [esi + 1ah], 0
loc_9_009E:
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        ret
loc_9_00A4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 4], 2
        mov    eax, dword ptr [esi + 20h]
        jne    SHORT loc_9_00C3
        push   0
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_00C0:
        add    esp, 8
loc_9_00C3:
        mov    word ptr [esi + 1ah], 0
        xor    eax, eax
        pop    esi
        ret
loc_9_00CD:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 1ah], 1
        cmp    byte ptr [esi + 4], 23h
        jne    SHORT loc_9_011A
        mov    edx, dword ptr [esi]
        cmp    edx, 1
        jne    SHORT loc_9_011A
        mov    ecx, dword ptr [esi + 20h]
        push   dword ptr [ecx + 4]
        push   dword ptr [esi + 14h]
        push   edx
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_00F6:
        test   eax, 80000000h
        jne    SHORT loc_9_010E
        mov    dword ptr [esi + 24h], 0cccccccch
        mov    word ptr [esi + 1ah], 0
        mov    dword ptr [esi], eax
        jmp    SHORT loc_9_011A
loc_9_010E:
        mov    word ptr [esi + 1ah], 1
        mov    dword ptr [esi], 0
loc_9_011A:
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        ret
loc_9_0120:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 20h]
        push   dword ptr [ecx + 4]
        push   dword ptr [esi + 14h]
        push   dword ptr [esi]
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0137:
        test   eax, 80000000h
        jne    SHORT loc_9_014A
        mov    dword ptr [esi], eax
        add    esi, 1ah
        mov    word ptr [esi], 0
        jmp    SHORT loc_9_0158
loc_9_014A:
        mov    dword ptr [esi], 0
        add    esi, 1ah
        mov    word ptr [esi], 1
loc_9_0158:
        movsx  eax, word ptr [esi]
        pop    esi
        ret
loc_9_015D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 20h]
        push   dword ptr [ecx + 4]
        push   dword ptr [esi + 14h]
        push   dword ptr [esi]
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0174:
        test   eax, 80000000h
        jne    SHORT loc_9_0187
        mov    dword ptr [esi], eax
        add    esi, 1ah
        mov    word ptr [esi], 0
        jmp    SHORT loc_9_0195
loc_9_0187:
        mov    dword ptr [esi], 0
        add    esi, 1ah
        mov    word ptr [esi], 1
loc_9_0195:
        movsx  eax, word ptr [esi]
        pop    esi
        ret
loc_9_019A:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [edx + 28h]
        mov    ebx, dword ptr [edx + 20h]
        mov    al, byte ptr [edx + 4]
        test   al, al
        jne    SHORT loc_9_021A
        mov    word ptr [esi + 14h], 80a0h
        mov    ecx, dword ptr [ebx + 4]
        test   ecx, ecx
        jne    SHORT loc_9_01C2
        mov    word ptr [esi + 14h], 80a1h
loc_9_01C2:
        cmp    ecx, 1
        jne    SHORT loc_9_01CB
        or     byte ptr [esi + 14h], 42h
loc_9_01CB:
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, edi
        and    eax, 0ffff0000h
        cmp    eax, 50490000h
        jne    SHORT loc_9_0224
        mov    eax, dword ptr [esi + 10h]
        mov    ebp, eax
        and    ebp, 0ffff0000h
        cmp    ebp, 50450000h
        jne    SHORT loc_9_0224
        or     edi, 20200000h
        or     eax, 20200000h
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [esi + 1ch], edi
        shl    ecx, 10h
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esi + 18h], ebx
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 14h], ecx
        jmp    SHORT loc_9_0224
loc_9_021A:
        cmp    al, 0ah
        jne    SHORT loc_9_0224
        mov    word ptr [esi + 14h], 10e0h
loc_9_0224:
        and    byte ptr [esi + 2ch], 0feh
        mov    word ptr [edx + 1ah], 0
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_9_0235:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 0
        xor    eax, eax
        ret
sub_9_0242:
        push   esi
        push   1
        push   0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_024C:
        add    esp, 8
        mov    esi, eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0256:
        test   esi, esi
        je     SHORT loc_9_026D
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [eax], esi
        xor    eax, eax
        jmp    SHORT loc_9_0272
loc_9_026D:
        mov    eax, 80000000h
loc_9_0272:
        pop    esi
        ret    8
sub_9_0276:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_9_029B
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_9_029B
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_028E:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
loc_9_029B:
        ret
_PTEXT ends
        end
