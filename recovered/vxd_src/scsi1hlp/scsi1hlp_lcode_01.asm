; scsi1hlp_lcode_01.asm -- SCSI1HLP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

scsi1hlp_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        db  00Fh, 084h, 029h, 002h, 0FFh, 0FFh
        cmp    si, 4
        db  00Fh, 084h, 03Ah, 004h, 0FFh, 0FFh
        cmp    si, 2
        db  00Fh, 084h, 052h, 004h, 0FFh, 0FFh
        mov    word ptr [ebx + 2], 0ffffh
        ret
        db  000h, 000h
loc_1_0040:
        push   esi
        mov    edx, dword ptr [esp + 8]
        db  0BEh, 09Ch, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        ret
loc_1_0052:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 8]
        mov    ecx, dword ptr [ecx + 8]
loc_1_005D:
        db  081h, 039h, 0C4h, 000h, 000h, 000h
        je     SHORT loc_1_006C
        mov    ecx, dword ptr [ecx + 0ch]
        or     ecx, ecx
        jne    SHORT loc_1_005D
loc_1_006C:
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [eax + 4], ecx
        movsx  eax, word ptr [ecx + 10h]
        add    eax, edx
        or     byte ptr [eax + 0dh], 1
        db  0BEh, 09Ch, 004h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        ret
        db  000h, 000h, 000h
sub_1_008C:
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
        db  077h, 001h
        add    byte ptr [eax], al
loc_1_00C4:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 4]
        cmp    word ptr [ebx + 68h], 0fh
        je     SHORT loc_1_00E0
        test   byte ptr [ebx + 6dh], 80h
        jne    NEAR PTR loc_1_0168
loc_1_00E0:
        mov    al, 4
        movzx  ecx, word ptr [ebx + 68h]
        cmp    ecx, 0eh
        jae    SHORT loc_1_00F5
        db  0FFh, 014h, 08Dh, 08Ch, 000h, 000h, 000h
loc_1_00F5:
        mov    edx, dword ptr [ebx + 10h]
        or     al, al
        je     SHORT loc_1_010D
        dec    al
        jne    SHORT loc_1_0130
        mov    eax, dword ptr [ebx + 10h]
        movsx  eax, word ptr [eax + 10h]
        add    eax, ebx
        and    byte ptr [eax + 0dh], 0feh
loc_1_010D:
        mov    eax, dword ptr [ebx + 14h]
        db  0C7h, 000h, 040h, 000h, 000h, 000h
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 14h], 8
loc_1_011D:
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ebx + 10h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_012C:
        pop    esi
        pop    edi
        pop    ebp
        ret
loc_1_0130:
        cmp    al, 3
        je     SHORT loc_1_011D
        dec    al
        jne    SHORT loc_1_0149
        mov    eax, dword ptr [ebx + 14h]
        sub    eax, 8
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        jmp    SHORT loc_1_012C
loc_1_0149:
        dec    al
        je     SHORT loc_1_012C
        mov    al, 1
        mov    word ptr [ebx + 6ah], 16h
        cmp    word ptr [ebx + 68h], 0dh
        jne    SHORT loc_1_0130
        mov    dword ptr [ebx + 0a0h], 1fh
        jmp    SHORT loc_1_0130
loc_1_0168:
        db  0BEh, 008h, 007h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_012C
loc_1_0177:
        mov    al, 4
        ret
        db  000h, 000h
sub_1_017C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0188:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0190:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 073h, 063h, 073h, 069h
        db  031h, 068h, 06Ch, 070h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
