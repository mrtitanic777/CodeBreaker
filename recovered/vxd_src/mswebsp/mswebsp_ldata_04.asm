; mswebsp_ldata_04.asm -- MSWEBSP object 4 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h
        db  073h, 05Ch, 057h, 033h, 053h, 076h, 063h, 05Ch, 055h, 073h, 065h, 072h, 073h, 05Ch, 000h, 000h
        db  05Ch, 047h, 072h, 06Fh, 075h, 070h, 073h, 000h
sub_4_0038:
        rol    byte ptr [edx], 0
        db  000h, 0E0h
        add    al, byte ptr [eax]
        db  000h, 040h, 003h
        add    byte ptr [eax], al
        pushad
        add    eax, dword ptr [eax]
        db  000h, 080h, 003h, 000h, 000h, 0D0h
        add    eax, dword ptr [eax]
        db  000h, 0E0h
        add    eax, dword ptr [eax]
        db  000h, 0E0h
        add    ebp, dword ptr [ecx]
        push   eax
_LDATA ends
        end
