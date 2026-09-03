; vmpoll_pcode_07.asm -- VMPOLL object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VMPoll_Get_Version:
        mov    eax, 400h
loc_7_0005:
        clc
        ret
VMPoll_Enable_Disable:
        test   ebx, ebx
        jne    SHORT sub_7_002C
        db  0A2h, 008h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
loc_7_0016:
        call   sub_7_002C
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_7_0016
        xor    ebx, ebx
        ret
sub_7_002C:
        push   ebx
        push   eax
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  0C7h, 003h, 001h, 000h, 000h, 000h
        or     byte ptr [ebx + 10h], 2
        pop    eax
        test   al, al
        jne    SHORT loc_7_0052
        db  0C7h, 003h, 046h, 000h, 000h, 000h
loc_7_0052:
        pop    ebx
        ret
loc_7_0054:
        db  003h, 01Dh, 080h, 001h, 000h, 000h
        db  0BEh, 001h, 000h, 000h, 000h
        db  080h, 03Dh, 008h, 000h, 000h, 000h, 000h
        jne    SHORT loc_7_006D
        db  0BEh, 046h, 000h, 000h, 000h
loc_7_006D:
        mov    dword ptr [ebx], esi
        clc
        ret
loc_7_0071:
        movzx  eax, word ptr [edx + 0ch]
        push   eax
        db  0B9h, 046h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_7_008F
        imul   eax, eax, 14h
        cdq
        mov    ecx, 32h
        div    ecx
        db  0B9h, 001h, 000h, 000h, 000h
loc_7_008F:
        db  08Bh, 015h, 080h, 001h, 000h, 000h
        mov    dword ptr [edx + ebx + 0ch], eax
        mov    dword ptr [edx + ebx], ecx
        pop    eax
        and    byte ptr [edx + ebx + 10h], 0efh
        cmp    eax, 21h
        jbe    SHORT loc_7_00AC
        or     byte ptr [edx + ebx + 10h], 10h
loc_7_00AC:
        and    dword ptr [edx + ebx + 4], 0
        db  0CDh, 020h, 039h, 080h, 001h, 000h
_PTEXT ends
        end
