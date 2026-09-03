; nwsp_ldata_04.asm -- NWSP object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  047h, 052h, 04Fh, 055h, 050h, 053h, 05Fh, 049h, 027h, 04Dh, 05Fh, 049h, 04Eh, 000h, 000h, 000h
sub_4_0010:
        lock or byte ptr [eax], al
        db  000h, 010h
        or     dword ptr [eax], eax
        db  000h, 070h, 009h
        add    byte ptr [eax], al
        nop
        or     dword ptr [eax], eax
        db  000h, 0B0h, 009h, 000h, 000h, 000h
        or     al, byte ptr [eax]
        db  000h, 030h
        or     al, byte ptr [eax]
        db  000h, 030h
        or     ch, byte ptr [esi]
        push   eax
_LDATA ends
        end
