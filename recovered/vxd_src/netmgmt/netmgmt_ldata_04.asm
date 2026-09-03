; netmgmt_ldata_04.asm -- NETMGMT object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  058h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 0F9h, 000h, 000h, 000h, 05Dh, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_4_0014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LDATA ends
        end
