; configmg_pcode_03.asm -- CONFIGMG object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0CDh, 020h, 000h, 000h, 040h, 000h, 072h, 016h, 053h, 033h, 0DBh, 06Ah, 000h, 053h, 0CDh, 020h
        db  04Ch, 000h, 040h, 000h, 083h, 0C4h, 008h, 043h, 083h, 0FBh, 01Ah, 072h, 0EEh, 05Bh, 0C3h
loc_3_001F:
        push   esi
        xor    edx, edx
        mov    esi, dword ptr [esp + 8]
        push   0
        push   esi
        mov    ecx, 0dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0034:
        push   1
        xor    edx, edx
        push   esi
        mov    ecx, 0dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0044:
        mov    eax, 1
        pop    esi
        ret
loc_3_004B:
        push   esi
        xor    edx, edx
        mov    esi, dword ptr [esp + 8]
        push   0
        push   esi
        mov    ecx, 12h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0060:
        push   1
        xor    edx, edx
        push   esi
        mov    ecx, 12h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0070:
        mov    eax, 1
        pop    esi
        ret
loc_3_0077:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 010h
        je     SHORT loc_3_0097
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0094:
        add    esp, 4
loc_3_0097:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_3_00AC
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00AC:
        call   0
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00BD:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_3_00ED
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00D2:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00E1:
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00ED:
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 040h
        ret
sub_3_00F5:
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        xor    edx, edx
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 008h
        push   eax
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_010D:
        mov    eax, 1
        ret
sub_3_0113:
        db  081h, 00Dh, 000h, 000h, 000h, 000h, 008h, 004h, 000h, 000h
        xor    eax, eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   eax
        xor    edx, edx
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h
        push   eax
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0142:
        mov    eax, 1
        ret
_PTEXT ends
        end
