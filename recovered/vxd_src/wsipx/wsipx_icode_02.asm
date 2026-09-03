; wsipx_icode_02.asm -- WSIPX object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0B8h, 07Eh, 004h, 000h, 000h
        movzx  edx, byte ptr [eax]
        or     edx, edx
        jne    SHORT loc_2_0088
        inc    byte ptr [eax]
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        cmp    ax, 400h
        jb     SHORT loc_2_0088
        db  0CDh, 020h, 000h, 000h, 017h, 000h
        jb     SHORT loc_2_0088
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     SHORT loc_2_0088
        db  0CDh, 020h, 000h, 000h, 03Eh, 000h
        jb     SHORT loc_2_0088
        mov    eax, 3fh
        mov    ebx, 1
        db  0B9h, 054h, 026h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 03Eh, 000h
        jb     SHORT loc_2_0088
        or     eax, eax
        je     SHORT loc_2_0088
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_006B
        db  0FEh, 005h, 07Ch, 004h, 000h, 000h
        push   ebx
        xor    ebx, ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        db  088h, 01Dh, 07Dh, 004h, 000h, 000h
        pop    ebx
loc_2_006B:
        mov    ebx, 1ah
        db  0CDh, 020h, 001h, 000h, 087h, 004h
        jb     SHORT loc_2_0088
        db  0A3h, 0CCh, 026h, 000h, 000h
        db  0E8h, 0F6h, 024h, 0FEh, 0FFh
        or     eax, eax
        jne    SHORT loc_2_0088
        clc
        ret
loc_2_0088:
        stc
        ret
loc_2_008A:
        db  0E8h, 0D6h, 000h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_2_0095
        clc
        ret
loc_2_0095:
        stc
        ret
        db  022h
_ITEXT ends
        end
