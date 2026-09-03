; ircomm_icode_04.asm -- IRCOMM object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_4_0000:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_004C
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        jb     SHORT loc_4_004E
        mov    eax, 4
        mov    ecx, 454h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_4_004E
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 02Bh, 000h
        add    esp, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0048:
        or     eax, eax
        je     SHORT loc_4_004E
loc_4_004C:
        clc
        ret
loc_4_004E:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        stc
        ret
sub_4_005A:
        push   ebp
        mov    ebp, esp
        push   ebx
        xor    ebx, ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx], ebx
        pop    ebx
        pop    ebp
        ret
_ITEXT ends
        end
