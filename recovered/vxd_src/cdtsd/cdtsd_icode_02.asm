; cdtsd_icode_02.asm -- CDTSD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0C7h, 005h, 034h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0C7h, 005h, 038h, 000h, 000h, 000h, 044h, 000h, 000h, 000h
        db  068h, 028h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        xor    eax, eax
        ret
        db  000h, 000h, 000h
loc_2_0028:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 44h], al
        sub    eax, 204d4f52h
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_ITEXT ends
        end
