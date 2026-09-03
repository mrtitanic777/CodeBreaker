; vmcpd_pcode_03.asm -- VMCPD object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VMCPD_Service_10:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_0081
        push   edi
        db  039h, 03Dh, 064h, 008h, 000h, 000h
        je     SHORT loc_3_004E
        mov    eax, edi
        db  003h, 005h, 070h, 008h, 000h, 000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_3_0044
        db  083h, 03Dh, 064h, 008h, 000h, 000h, 000h
        pushfd
        or     ecx, 0ffffffffh
        db  0E8h, 02Fh, 007h, 0FDh, 0FFh
        popfd
        je     SHORT loc_3_004E
        mov    edi, dword ptr [esp]
loc_3_0044:
        db  0E8h, 0C2h, 002h, 0FDh, 0FFh
        db  0E8h, 057h, 002h, 0FDh, 0FFh
loc_3_004E:
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    esi, dword ptr [edi]
        fnsave dword ptr [esi + 4]
        add    esi, 4
        mov    edi, dword ptr [ebp + 8]
        movsd
        movsd
        add    esi, 4
        mov    ecx, 5
        rep movsd
        pop    edi
        mov    eax, 1
        db  0E8h, 0D7h, 001h, 0FDh, 0FFh
        wait
        fninit
        wait
        mov    eax, 0
        jmp    SHORT loc_3_0086
loc_3_0081:
        mov    eax, 0c000014ah
loc_3_0086:
        pop    esi
        pop    edi
        leave
        ret
VMCPD_Service_11:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0F7h, 005h, 040h, 008h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_00E1
        mov    eax, cr0
        db  039h, 03Dh, 064h, 008h, 000h, 000h
        je     SHORT loc_3_00B1
        db  0E8h, 05Ah, 002h, 0FDh, 0FFh
loc_3_00B1:
        push   edi
        db  003h, 03Dh, 070h, 008h, 000h, 000h
        mov    edi, dword ptr [edi]
        add    edi, 4
        mov    eax, edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        movsd
        movsd
        add    edi, 4
        mov    ecx, 5
        rep movsd
        mov    dword ptr [edx + 18h], 0ffffffffh
        mov    dword ptr [eax + 0ch], 0ffffh
        pop    edi
        jmp    SHORT loc_3_00E8
loc_3_00E1:
        mov    eax, 0c0000001h
        jmp    SHORT loc_3_00ED
loc_3_00E8:
        mov    eax, 0
loc_3_00ED:
        pop    esi
        pop    edi
        leave
        ret
_PTEXT ends
        end
