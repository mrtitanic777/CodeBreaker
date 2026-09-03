; apix_icode_02.asm -- APIX object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0F8h, 0C3h
APIX_SYS_DYNAMIC_DEVICE_INIT:
        db  0C7h, 005h, 038h, 000h, 000h, 000h, 0B0h, 001h, 000h, 000h
        db  0C7h, 005h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  068h, 02Ch, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 010h, 000h
        pop    eax
        clc
        ret
        db  083h, 0C4h, 010h, 0F8h, 0C3h, 000h, 000h, 000h
loc_2_002C:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
loc_2_0038:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_003C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        and    byte ptr [ecx + 50h], al
        dec    ecx
        and    byte ptr [eax + 4ch], bl
        inc    ecx
        push   esp
        dec    edi
        push   edx
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
