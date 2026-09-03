; netmgmt_icode_02.asm -- NETMGMT object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

sub_2_0000:
        push   1
        push   0
        db  0E8h, 0F7h, 0FFh, 0FDh, 0FFh
        add    esp, 8
        db  0A3h, 010h, 000h, 000h, 000h
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_2_001F
        mov    eax, 1
loc_2_001F:
        ret
_ITEXT ends
        end
