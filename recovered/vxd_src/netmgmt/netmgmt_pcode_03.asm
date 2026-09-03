; netmgmt_pcode_03.asm -- NETMGMT object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0CDh, 020h, 055h, 080h, 028h, 000h, 0CDh, 020h, 030h, 080h, 028h, 000h
sub_3_000C:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_0020:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        jne    SHORT loc_3_002C
        sub    eax, eax
        jmp    SHORT loc_3_0055
loc_3_002C:
        cmp    ecx, -1
        jne    SHORT loc_3_0038
        call   sub_3_01AD
        jmp    SHORT loc_3_0055
loc_3_0038:
        cmp    ecx, 4
        mov    eax, 32h
        ja     SHORT loc_3_0055
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0FFh, 014h, 08Dh, 0FCh, 0FFh, 0FFh, 0FFh
loc_3_0055:
        ret    10h
loc_3_0058:
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        cmp    dword ptr [eax + 1ch], 4
        jae    SHORT loc_3_006C
        mov    eax, 0eah
        jmp    SHORT loc_3_007D
loc_3_006C:
        or     ecx, ecx
        mov    eax, 57h
        je     SHORT loc_3_007D
        mov    dword ptr [ecx], 100h
        sub    eax, eax
loc_3_007D:
        ret    0ch
loc_3_0080:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        mov    ebx, 0c0000001h
        mov    ebp, dword ptr [edi + 10h]
        or     ebp, ebp
        je     SHORT loc_3_00F0
        cmp    dword ptr [edi + 14h], 10h
        jb     SHORT loc_3_00F0
        cmp    dword ptr [edi + 18h], 0
        je     SHORT loc_3_00F0
        mov    esi, dword ptr [edi + 1ch]
        or     esi, esi
        je     SHORT loc_3_00F0
        push   dword ptr [ebp + 4]
        call   sub_3_0319
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_00BC
        mov    eax, 0c0010006h
        jmp    SHORT loc_3_00F2
loc_3_00BC:
        push   10h
        db  0FFh, 035h, 010h, 000h, 000h, 000h
        db  0E8h, 047h, 0FFh, 0FCh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [eax], esi
        push   dword ptr [edi + 20h]
        push   dword ptr [edi + 18h]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_0245
        mov    ebx, eax
        db  0FFh, 035h, 010h, 000h, 000h, 000h
        db  0E8h, 02Bh, 0FFh, 0FCh, 0FFh
        add    esp, 4
loc_3_00F0:
        mov    eax, ebx
loc_3_00F2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_3_00F9:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        mov    edi, 0c0000001h
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        je     SHORT loc_3_0156
        cmp    dword ptr [eax + 14h], 10h
        jb     SHORT loc_3_0156
        push   dword ptr [esi + 4]
        call   sub_3_0319
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_0126
        mov    eax, 0c0010006h
        jmp    SHORT loc_3_0158
loc_3_0126:
        push   10h
        db  0FFh, 035h, 010h, 000h, 000h, 000h
        db  0E8h, 0DDh, 0FEh, 0FCh, 0FFh
        add    esp, 8
        push   dword ptr [esi + 0ch]
        lea    eax, [esi + 10h]
        push   eax
        push   dword ptr [esi + 8]
        push   edi
        call   sub_3_02BC
        mov    edi, eax
        db  0FFh, 035h, 010h, 000h, 000h, 000h
        db  0E8h, 0C5h, 0FEh, 0FCh, 0FFh
        add    esp, 4
loc_3_0156:
        mov    eax, edi
loc_3_0158:
        pop    edi
        pop    esi
        ret    0ch
loc_3_015D:
        push   esi
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 18h]
        cmp    dword ptr [eax + 1ch], 9
        jae    SHORT loc_3_0172
        mov    eax, 0eah
        jmp    SHORT loc_3_0195
loc_3_0172:
        mov    eax, dword ptr [eax + 10h]
        push   dword ptr [eax]
        call   sub_3_0319
        or     eax, eax
        jne    SHORT loc_3_0187
        mov    eax, 0c0010006h
        jmp    SHORT loc_3_0195
loc_3_0187:
        mov    cl, byte ptr [eax + 3ch]
        mov    byte ptr [esi + 4], cl
        mov    eax, dword ptr [eax + 74h]
        mov    dword ptr [esi + 5], eax
        sub    eax, eax
loc_3_0195:
        pop    esi
        ret    0ch
sub_3_0199:
        db  0FFh, 035h, 010h, 000h, 000h, 000h
        db  0E8h, 064h, 0FEh, 0FCh, 0FFh
        add    esp, 4
        mov    eax, 1
        ret
sub_3_01AD:
        mov    eax, 1
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_01C0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi]
        db  0A3h, 0E8h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        mov    edi, dword ptr [eax + 18h]
        or     edi, edi
        jne    SHORT loc_3_01E2
        mov    dword ptr [esi], 0c0000001h
        jmp    SHORT loc_3_023E
loc_3_01E2:
        mov    eax, dword ptr [edi + 1ch]
        mov    ecx, dword ptr [eax + 50h]
        add    eax, 50h
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        db  0C7h, 000h, 040h, 000h, 000h, 000h
        mov    ebp, dword ptr [esp + 1ch]
        push   ebp
        push   edi
        push   esi
        call   6
        mov    ebx, dword ptr [esi]
        cmp    ebx, 103h
        jne    SHORT loc_3_0233
        mov    ebp, dword ptr [ebp + 10h]
        cmp    ebp, -1
        je     SHORT loc_3_0224
loc_3_0215:
        push   2710h
        call   sub_3_000C
        cmp    ebp, -1
        jne    SHORT loc_3_0215
loc_3_0224:
        cmp    ebx, 103h
        jne    SHORT loc_3_0233
        db  0A1h, 0E8h, 000h, 000h, 000h
        mov    dword ptr [esi], eax
loc_3_0233:
        mov    ecx, dword ptr [edi + 1ch]
        db  0A1h, 014h, 000h, 000h, 000h
        mov    dword ptr [ecx + 50h], eax
loc_3_023E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_3_0245:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        db  0BFh, 0C0h, 000h, 000h, 000h
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        db  0C7h, 005h, 0D0h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 0D4h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 1ch]
        db  0A3h, 0D8h, 000h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  068h, 0C0h, 000h, 000h, 000h
        db  0A3h, 0DCh, 000h, 000h, 000h
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_3_01C0
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_3_02AA
        db  0A1h, 0E0h, 000h, 000h, 000h
        mov    dword ptr [esi], eax
        jmp    SHORT loc_3_02B0
loc_3_02AA:
        mov    dword ptr [esi], 0
loc_3_02B0:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 4
        ret    10h
sub_3_02BC:
        sub    esp, 4
        push   edi
        mov    dword ptr [esp + 4], 0
        db  0BFh, 0C0h, 000h, 000h, 000h
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        db  0C7h, 005h, 0D0h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        db  0A3h, 0D4h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 0D8h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 18h]
        db  068h, 0C0h, 000h, 000h, 000h
        db  0A3h, 0DCh, 000h, 000h, 000h
        lea    eax, [esp + 8]
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_3_01C0
        mov    eax, dword ptr [esp + 4]
        pop    edi
        add    esp, 4
        ret    10h
sub_3_0319:
        sub    esp, 4
        lea    eax, [esp]
        push   0
        push   eax
        call   0
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 8
        sub    edx, edx
loc_3_0331:
        cmp    dword ptr [esp], edx
        je     SHORT loc_3_0351
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp], eax
        cmp    dword ptr [eax], 5044414eh
        jne    SHORT loc_3_0331
        cmp    dword ptr [eax + 38h], ecx
        jne    SHORT loc_3_0331
        jmp    SHORT loc_3_0353
loc_3_0351:
        sub    eax, eax
loc_3_0353:
        add    esp, 4
        ret    4
_PTEXT ends
        end
