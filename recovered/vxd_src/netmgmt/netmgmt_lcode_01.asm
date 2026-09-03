; netmgmt_lcode_01.asm -- NETMGMT object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

NETMGMT_Control:
        cmp    eax, 2
        jne    SHORT loc_1_000E
        db  0E8h, 0F6h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        ret
loc_1_000E:
        cmp    eax, 1bh
        jne    SHORT loc_1_001C
        db  0E8h, 0E8h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        ret
loc_1_001C:
        cmp    eax, 1ch
        jne    SHORT loc_1_002A
        db  0E8h, 073h, 001h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_002A:
        cmp    eax, 23h
        jne    SHORT loc_1_003C
        push   esi
        push   edx
        push   ebx
        push   ecx
        db  0E8h, 0E8h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        ret
loc_1_003C:
        clc
        ret
        db  000h, 000h
loc_1_0040:
        mov    ecx, dword ptr [esp + 8]
        db  081h, 0F9h, 0C0h, 000h, 000h, 000h
        jne    SHORT loc_1_005E
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 0E8h, 000h, 000h, 000h
        mov    dword ptr [ecx + 10h], 0ffffffffh
        jmp    SHORT loc_1_006D
loc_1_005E:
        push   dword ptr [esp + 0ch]
        push   ecx
        push   dword ptr [esp + 0ch]
        db  0FFh, 015h, 014h, 000h, 000h, 000h
loc_1_006D:
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 04Eh, 045h, 054h, 04Dh
        db  047h, 04Dh, 054h, 020h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
