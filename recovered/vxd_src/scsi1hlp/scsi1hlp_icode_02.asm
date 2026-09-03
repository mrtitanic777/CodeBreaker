; scsi1hlp_icode_02.asm -- SCSI1HLP object 2 segment _ITEXT
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
        db  000h, 080h, 000h, 000h, 00Ch, 000h
        add    byte ptr [eax], al
        db  07Ch, 001h
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        xor    dword ptr [eax], esp
        inc    ebx
        inc    esp
        dec    eax
        inc    ebp
        dec    esp
        push   eax
        and    byte ptr [esi + 53h], dl
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
