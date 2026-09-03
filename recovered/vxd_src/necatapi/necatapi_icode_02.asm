; necatapi_icode_02.asm -- NECATAPI object 2 segment _ITEXT
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
        db  000h, 00Ch, 000h
        add    byte ptr [eax], al
        and    al, 3
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        inc    ebx
        and    byte ptr [ebx + 44h], al
        push   edx
        xor    dh, byte ptr [esi]
        xor    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   esi
        push   ebx
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
