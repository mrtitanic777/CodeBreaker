; bios_lcode_01.asm -- BIOS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 03Dh, 000h, 004h, 000h, 000h, 000h, 042h, 049h, 04Fh, 053h
        db  020h, 020h, 020h, 020h, 001h, 000h, 080h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
BIOS_Control:
        cmp    eax, 22h
        jne    SHORT loc_1_0076
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0074:
        stc
        ret
loc_1_0076:
        cmp    eax, 1
        jne    SHORT loc_1_0089
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0085:
        cmp    eax, 1
        ret
loc_1_0089:
        cmp    eax, 23h
        jne    SHORT loc_1_0098
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0094:
        cmp    eax, 1
        ret
loc_1_0098:
        clc
        ret
loc_1_009A:
        push   ebx
        push   esi
        push   edi
        pushad
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        in     al, dx
        test   al, 1
        mov    bl, al
        je     SHORT loc_1_00CD
        push   0
        and    bl, 0feh
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_0100
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
        mov    al, bl
        out    dx, al
        stc
        jmp    SHORT loc_1_00CE
loc_1_00CD:
        clc
loc_1_00CE:
        popad
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_00D3:
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        test   byte ptr [eax], 1
        je     SHORT loc_1_00F0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_0100
        add    esp, 0ch
        jmp    SHORT loc_1_00FF
loc_1_00F0:
        push   0fah
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_00FF:
        ret
sub_1_0100:
        db  0CDh, 020h, 04Eh, 080h, 033h, 000h
        int3
        int3
_LTEXT ends
        end
