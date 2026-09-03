; vcdfsd_lcode_01.asm -- VCDFSD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VCDFSD_Control:
        cmp    eax, 2
        jne    SHORT loc_1_000A
        db  0E8h, 0F6h, 0FFh, 0FEh, 0FFh
loc_1_000A:
        push   eax
        db  0A1h, 0CCh, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        xchg   dword ptr [esp], eax
        ret
loc_1_0017:
        db  0A1h, 0CCh, 000h, 000h, 000h
        jmp    dword ptr [eax + 0ch]
loc_1_001F:
        mov    eax, 0bh
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [ecx + 1ah], ax
        ret
VCDFSD_Service_00:
        mov    eax, 400h
loc_1_0032:
        clc
        ret
VCDFSD_Service_01:
        db  0B8h, 0BCh, 000h, 000h, 000h
        ret
loc_1_003A:
        db  0A1h, 0CCh, 000h, 000h, 000h
        jmp    dword ptr [eax + 14h]
loc_1_0042:
        cmp    byte ptr [ebp + 1dh], 15h
        je     SHORT loc_1_004A
loc_1_0048:
        stc
        ret
loc_1_004A:
        db  066h, 083h, 03Dh, 0C4h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_0048
        db  0E9h, 0CBh, 0FFh, 0FEh, 0FFh
        db  000h, 000h, 000h, 02Dh, 000h, 000h, 000h, 034h, 000h, 000h, 000h, 053h, 000h, 000h, 000h, 09Ah
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 041h, 000h, 003h, 000h, 000h, 000h, 056h, 043h, 044h, 046h
        db  053h, 044h, 020h, 020h, 000h, 000h, 0F0h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  05Ch, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  004h, 000h, 0C0h, 000h, 000h, 000h, 022h, 000h, 000h, 000h
loc_1_00C6:
        pop    edi
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_00CC:
        db  07Ch, 001h
        add    byte ptr [eax], al
loc_1_00D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_017C:
        db  07Ch, 001h
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    al, byte ptr [eax]
        lea    eax, [eax]
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [eax], al
        db  000h, 068h, 002h
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [eax], al
        db  000h, 0CFh
        add    byte ptr [eax], al
        db  000h, 05Dh, 001h
        add    byte ptr [eax], al
_LTEXT ends
        end
