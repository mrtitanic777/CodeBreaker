; filesec_icode_02.asm -- FILESEC object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

FILESEC_DEVICE_INIT:
        call   sub_2_0100
        call   sub_2_00AD
        or     eax, eax
        jne    SHORT loc_2_0010
        clc
        ret
loc_2_0010:
        stc
        ret
FILESEC_INIT_COMPLETE:
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jae    SHORT loc_2_001C
        jmp    SHORT loc_2_0046
loc_2_001C:
        db  0CDh, 020h, 000h, 000h, 093h, 004h
        jae    SHORT loc_2_0026
        jmp    SHORT loc_2_0046
loc_2_0026:
        call   sub_2_0070
        or     eax, eax
        je     SHORT loc_2_0031
        jmp    SHORT loc_2_0046
loc_2_0031:
        db  0CDh, 020h, 000h, 000h, 032h, 000h
        jb     SHORT loc_2_0044
        call   sub_2_00D0
        or     eax, eax
        jne    SHORT loc_2_0044
        jmp    SHORT loc_2_0046
loc_2_0044:
        clc
        ret
loc_2_0046:
        stc
        ret
sub_2_0048:
        push   esi
        mov    ax, word ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 032h, 000h
        jb     SHORT loc_2_0061
        mov    eax, 1
        jmp    SHORT loc_2_0063
loc_2_0061:
        xor    eax, eax
loc_2_0063:
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0070:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        lea    eax, [ebp - 20h]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 005h, 000h, 093h, 004h
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_00A9
        db  068h, 0F0h, 002h, 000h, 000h
        push   dword ptr [ebp - 18h]
        db  0E8h, 0F5h, 020h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 1ch]
        db  0A3h, 070h, 003h, 000h, 000h
        sub    eax, eax
loc_2_00A9:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_00AD:
        push   1
        push   0
        call   sub_2_0174
        add    esp, 8
        db  0A3h, 0ECh, 002h, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_00D0:
        push   esi
        db  0BEh, 034h, 001h, 000h, 000h
loc_2_00D6:
        push   esi
        push   dword ptr [esi - 4]
        call   sub_2_0048
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_00F8
        add    esi, 14h
        db  081h, 0FEh, 0ACh, 001h, 000h, 000h
        jb     SHORT loc_2_00D6
        mov    eax, 1
loc_2_00F6:
        pop    esi
        ret
loc_2_00F8:
        sub    eax, eax
        jmp    SHORT loc_2_00F6
        db  000h, 000h, 000h, 000h
sub_2_0100:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   edi
        db  083h, 03Dh, 0DCh, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_2_016D
        mov    dword ptr [ebp - 0ch], 5
        mov    dword ptr [ebp - 8], 0
        mov    ebx, 0ffffffffh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, 0ffffffffh
        lea    edi, [ebp - 14h]
        mov    eax, 6507h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jae    SHORT loc_2_0141
        mov    dword ptr [ebp - 8], eax
loc_2_0141:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_016D
        mov    eax, dword ptr [ebp - 13h]
        mov    dword ptr [ebp - 4], eax
        call   sub_2_017C
        movzx  ecx, word ptr [ebp - 2]
        shl    ecx, 4
        add    ecx, dword ptr [eax + 4]
        movzx  eax, word ptr [ebp - 4]
        cmp    word ptr [ecx + eax], 1
        sbb    eax, eax
        inc    eax
        db  0A3h, 0DCh, 001h, 000h, 000h
loc_2_016D:
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_0174:
        db  0CDh, 020h, 001h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_2_017C:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
_ITEXT ends
        end
