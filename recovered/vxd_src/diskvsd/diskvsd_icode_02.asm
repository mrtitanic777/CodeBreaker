; diskvsd_icode_02.asm -- DISKVSD object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0C7h, 005h, 04Ch, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  0C7h, 005h, 050h, 000h, 000h, 000h, 000h, 00Bh, 000h, 000h
        db  068h, 040h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        add    esp, 4
        db  066h, 083h, 03Dh, 06Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_2_003A
        db  066h, 083h, 03Dh, 06Ch, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_0038
        jmp    SHORT loc_2_003A
loc_2_0038:
        jmp    SHORT loc_2_003D
loc_2_003A:
        xor    eax, eax
        ret
loc_2_003D:
        stc
        ret
        db  000h
loc_2_0040:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_2_004C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        and    byte ptr [ecx + ebp*2 + 73h], al
        imul   esp, dword ptr [eax], 56h
        push   ebx
        inc    esp
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
