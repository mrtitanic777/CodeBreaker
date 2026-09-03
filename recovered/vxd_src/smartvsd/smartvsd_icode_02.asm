; smartvsd_icode_02.asm -- SMARTVSD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  068h, 024h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        db  066h, 083h, 03Dh, 050h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_0023
        db  066h, 083h, 03Dh, 050h, 000h, 000h, 000h, 002h
        je     SHORT loc_2_0023
        stc
loc_2_0023:
        ret
loc_2_0024:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        sub    al, 0
        add    byte ptr [eax], al
        js     SHORT loc_2_0047
        add    byte ptr [eax], al
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        and    byte ptr [esi + 53h], dl
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0047:
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
