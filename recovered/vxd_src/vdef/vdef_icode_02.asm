; vdef_icode_02.asm -- VDEF object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        clc
        ret
loc_2_0002:
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_002A
        db  0CDh, 020h, 000h, 000h, 010h, 000h
        jb     SHORT loc_2_002A
        push   1
        push   22h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 040h, 000h
        add    esp, 0ch
        cld
        call   sub_2_002D
loc_2_002A:
        ret
loc_2_002B:
        clc
        ret
sub_2_002D:
        db  0C7h, 005h, 064h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
        db  0C7h, 005h, 068h, 000h, 000h, 000h, 004h, 001h, 000h, 000h
        db  0C7h, 005h, 06Ch, 000h, 000h, 000h, 068h, 004h, 000h, 000h
        db  068h, 05Ch, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        clc
        ret
        db  000h
loc_2_005C:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
loc_2_0064:
        add    al, 0
        add    byte ptr [eax], al
loc_2_0068:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_006C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        popaw
        db  075h, 06Ch
        db  074h, 020h
        inc    esi
        imul   ebp, dword ptr [ebp + 53h], 447379h
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
