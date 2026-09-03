; int13_pcode_03.asm -- INT13 object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    edi, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [edi + 20h]
        and    eax, 0a001h
        cmp    eax, 8001h
        jne    SHORT loc_3_0054
sub_3_0012:
        movzx  eax, byte ptr [edi + 15h]
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_3_0024
        test   al, 80h
        je     SHORT loc_3_0056
        cmp    al, 87h
        jbe    SHORT loc_3_002E
loc_3_0024:
        cmp    byte ptr [edi + 40h], 0ah
        jne    SHORT loc_3_0056
        cmp    al, 7
        ja     SHORT loc_3_0056
loc_3_002E:
        rol    al, 1
        db  08Bh, 034h, 085h, 0EAh, 009h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_3_0058
        db  089h, 03Ch, 085h, 0EAh, 009h, 000h, 000h
        db  0FEh, 005h, 0E8h, 009h, 000h, 000h
        cmp    byte ptr [edi + 40h], 0ah
        jne    SHORT loc_3_0054
        db  0FEh, 005h, 0E9h, 009h, 000h, 000h
loc_3_0054:
        clc
        ret
loc_3_0056:
        stc
        ret
loc_3_0058:
        cmp    esi, edi
        je     SHORT loc_3_0054
        jmp    SHORT loc_3_0056
loc_3_005E:
        mov    edi, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [edi + 20h]
        and    eax, 0a001h
        cmp    eax, 8001h
        jne    SHORT loc_3_00AB
        movzx  eax, byte ptr [edi + 15h]
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_3_0082
        test   al, 80h
        je     SHORT loc_3_00AB
        cmp    al, 87h
        jbe    SHORT loc_3_008C
loc_3_0082:
        cmp    byte ptr [edi + 40h], 0ah
        jne    SHORT loc_3_00AB
        cmp    al, 7
        ja     SHORT loc_3_00AB
loc_3_008C:
        rol    al, 1
        db  0C7h, 004h, 085h, 0EAh, 009h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FEh, 00Dh, 0E8h, 009h, 000h, 000h
        cmp    byte ptr [edi + 40h], 0ah
        jne    SHORT loc_3_00AB
        db  0FEh, 005h, 0E9h, 009h, 000h, 000h
loc_3_00AB:
        ret
_PTEXT ends
        end
