; atapchng_icode_02.asm -- ATAPCHNG object 2 segment _ITEXT
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
        add    dword ptr [eax], eax
        or     al, 0
        add    byte ptr [eax], al
        sub    al, 5
        add    byte ptr [eax], al
        inc    ecx
        push   esp
        inc    ecx
        push   eax
        dec    ecx
        and    byte ptr [ebx + 44h], al
        and    byte ptr [ebx + 48h], al
        inc    ecx
        dec    esi
        inc    edi
        inc    ebp
        push   edx
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
