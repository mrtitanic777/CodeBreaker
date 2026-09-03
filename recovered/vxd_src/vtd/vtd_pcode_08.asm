; vtd_pcode_08.asm -- VTD object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_8_0008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_8_000C:
        cmp    ecx, 4
        jbe    SHORT loc_8_0017
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_8_0017:
        jb     SHORT loc_8_0076
        db  0BEh, 0ECh, 004h, 000h, 000h
        mov    ah, al
        mov    edi, dword ptr [esi + 8]
        and    ah, 0c2h
        add    edi, ebx
        cmp    ah, 0c2h
        je     SHORT loc_8_0050
        test   al, 0c0h
        jne    SHORT loc_8_0040
        test   al, 30h
        jne    SHORT loc_8_0042
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_8_0041
        jmp    NEAR PTR sub_8_0115
loc_8_0040:
        out    dx, al
loc_8_0041:
        ret
loc_8_0042:
        and    al, 0eh
        cmp    al, 4
        je     SHORT loc_8_004C
        cmp    al, 6
        jne    SHORT loc_8_0041
loc_8_004C:
        mov    byte ptr [edi + 17h], al
        ret
loc_8_0050:
        cmp    dword ptr [edi + 10h], 0
        jne    SHORT loc_8_0041
        test   al, 20h
        je     SHORT loc_8_0061
        push   eax
        call   sub_8_0115
        pop    eax
loc_8_0061:
        test   al, 10h
        jne    SHORT loc_8_0041
        mov    ecx, dword ptr [edi + 10h]
        mov    al, byte ptr [edi + 17h]
        or     al, 30h
        mov    byte ptr [ecx + edi + 14h], al
        inc    ecx
        mov    dword ptr [edi + 10h], ecx
        ret
loc_8_0076:
        in     al, dx
        ret
loc_8_0078:
        db  0BEh, 0ECh, 004h, 000h, 000h
        mov    edi, dword ptr [esi + 8]
        add    edi, ebx
        cmp    ecx, 4
        jbe    SHORT loc_8_008D
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_8_008D:
        jb     SHORT loc_8_00FD
        xor    dword ptr [edi], 1
        test   byte ptr [edi], 1
        jne    SHORT loc_8_00F9
        mov    byte ptr [edi + 5], al
        xor    ecx, ecx
        db  0CDh, 020h, 042h, 001h, 001h, 000h
        movzx  eax, ax
        shl    eax, 4
        mov    eax, dword ptr [eax - 9]
        xor    al, al
        cmp    eax, 505700h
        jne    SHORT loc_8_00BA
        xor    eax, eax
        mov    word ptr [edi + 4], ax
loc_8_00BA:
        movzx  eax, word ptr [edi + 4]
        db  0CDh, 020h, 009h, 000h, 005h, 000h
        dec    ax
        inc    eax
        mov    dword ptr [edi + 4], eax
        cmp    eax, 4a7h
        jb     SHORT loc_8_00DE
loc_8_00D1:
        mov    dword ptr [edi + 8], eax
        cmp    ebx, dword ptr [esi + 10h]
        jne    SHORT loc_8_00FC
        db  0E9h, 043h, 001h, 0F8h, 0FFh
loc_8_00DE:
        mov    eax, 4a7h
        bts    dword ptr [edi], 1
        jb     SHORT loc_8_00D1
        pushad
        push   0
        push   ebx
        db  0CDh, 020h, 019h, 000h, 017h, 000h
        add    esp, 8
        popad
        jmp    SHORT loc_8_00D1
loc_8_00F9:
        mov    byte ptr [edi + 4], al
loc_8_00FC:
        ret
loc_8_00FD:
        mov    ecx, dword ptr [edi + 10h]
        test   ecx, ecx
        je     SHORT loc_8_010D
        dec    ecx
        mov    al, byte ptr [ecx + edi + 14h]
        mov    dword ptr [edi + 10h], ecx
        ret
loc_8_010D:
        call   sub_8_0115
        jmp    SHORT loc_8_00FD
VTD_Adjust_VM_Count:
        ret
sub_8_0115:
        cmp    dword ptr [esi + 14h], ebx
        jne    SHORT loc_8_0134
        db  0E8h, 0CFh, 0FFh, 0F7h, 0FFh
        sti
        mov    ecx, eax
        cmp    byte ptr [edi + 17h], 4
        je     SHORT loc_8_0155
        add    ecx, ecx
        sub    ecx, dword ptr [edi + 8]
        jge    SHORT loc_8_0155
        add    ecx, dword ptr [edi + 8]
        jmp    SHORT loc_8_0155
loc_8_0134:
        db  0E8h, 0DAh, 0FFh, 0F7h, 0FFh
        sti
        sub    eax, ecx
        cmp    byte ptr [edi + 17h], 4
        je     SHORT loc_8_0144
        add    eax, eax
loc_8_0144:
        mov    ecx, dword ptr [edi + 4]
        test   cx, cx
        je     SHORT loc_8_0152
        xor    edx, edx
        idiv   ecx
        mov    eax, edx
loc_8_0152:
        sub    cx, ax
loc_8_0155:
        xchg   cl, ch
        mov    word ptr [edi + 14h], cx
        mov    dword ptr [edi + 10h], 2
        ret
        db  0EBh, 006h, 0FFh, 025h, 008h, 000h, 000h, 000h
loc_8_016B:
        pushad
        db  0BEh, 0ECh, 004h, 000h, 000h
        mov    eax, dword ptr [20h]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_8_01AA
        mov    eax, 8
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        jne    SHORT loc_8_01AA
        mov    edi, ebx
        add    edi, dword ptr [esi + 8]
        mov    eax, 10000h
        cmp    dword ptr [edi + 4], eax
        je     SHORT loc_8_01AA
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], eax
        cmp    ebx, dword ptr [esi + 10h]
        jne    SHORT loc_8_01AA
        db  0E8h, 077h, 000h, 0F8h, 0FFh
loc_8_01AA:
        popad
        db  0FFh, 025h, 008h, 020h, 056h, 054h
_PTEXT ends
        end
