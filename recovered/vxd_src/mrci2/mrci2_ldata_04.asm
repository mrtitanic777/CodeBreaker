; mrci2_ldata_04.asm -- MRCI2 object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  04Dh, 053h, 046h, 054h, 000h, 002h, 000h, 002h, 000h, 000h, 000h, 000h, 0EBh, 002h, 000h, 000h
        db  000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0EBh, 002h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0030:
        or     eax, dword ptr [ecx]
        add    byte ptr [eax], al
loc_4_0034:
        add    byte ptr [eax], al
loc_4_0036:
        add    byte ptr [eax], al
_LDATA ends
        end
