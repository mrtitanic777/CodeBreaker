; msopl_lcode_01.asm -- MSOPL object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 04Dh, 004h, 004h, 000h, 000h, 000h, 06Dh, 073h, 06Fh, 070h
        db  06Ch, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_005C:
        or     al, 0
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h
msopl_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0075:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_007E:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0087:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0090:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0099:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_00A2:
        clc
        ret
sub_1_00A4:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 3
        db  08Bh, 090h, 000h, 000h, 000h, 000h
        db  08Bh, 080h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp + 0ch], eax
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_00FB
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_00FB
loc_1_00D7:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_00EB
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_00F1
loc_1_00E7:
        popfd
        clc
        jmp    SHORT loc_1_00FF
loc_1_00EB:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_00E7
loc_1_00F1:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_00FB
        jmp    SHORT loc_1_00D7
loc_1_00FB:
        popfd
        xor    edi, edi
        stc
loc_1_00FF:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_1_0105:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_0132
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0132
loc_1_011D:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_0128
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_0136
loc_1_0128:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_0132
        jmp    SHORT loc_1_011D
loc_1_0132:
        popfd
        xor    esi, esi
        stc
loc_1_0136:
        pop    ebx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_0144:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0180
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0180
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_0180
loc_1_0165:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 10h], ebx
        jne    SHORT loc_1_0171
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0171:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_0180
        jmp    SHORT loc_1_0165
loc_1_0180:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
