; msmpu401_lcode_01.asm -- MSMPU401 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 04Ch, 004h, 004h, 000h, 000h, 000h, 06Dh, 073h, 06Dh, 070h
        db  075h, 034h, 030h, 031h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_005C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h
msmpu401_Control:
        cmp    eax, 8
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_007D:
        cmp    eax, 0bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0086:
        cmp    eax, 3
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_008F:
        cmp    eax, 1bh
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_0098:
        cmp    eax, 1ch
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_00A1:
        cmp    eax, 22h
        db  00Fh, 084h, 000h, 000h, 000h, 000h
loc_1_00AA:
        clc
        ret
sub_1_00AC:
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
        je     SHORT loc_1_0103
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_0103
loc_1_00DF:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0ch], 1
        jne    SHORT loc_1_00F3
        cmp    dword ptr [edx + edi], ebx
        jne    SHORT loc_1_00F9
loc_1_00EF:
        popfd
        clc
        jmp    SHORT loc_1_0107
loc_1_00F3:
        cmp    word ptr [edx + edi], bx
        je     SHORT loc_1_00EF
loc_1_00F9:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_0103
        jmp    SHORT loc_1_00DF
loc_1_0103:
        popfd
        xor    edi, edi
        stc
loc_1_0107:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret
sub_1_010D:
        push   eax
        push   ebx
        pushfd
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     SHORT loc_1_013A
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_013A
loc_1_0125:
        cmp    dword ptr [eax + 4], edi
        jne    SHORT loc_1_0130
        mov    esi, eax
        popfd
        clc
        jmp    SHORT loc_1_013E
loc_1_0130:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_1_013A
        jmp    SHORT loc_1_0125
loc_1_013A:
        popfd
        xor    esi, esi
        stc
loc_1_013E:
        pop    ebx
        pop    eax
        ret
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_014C:
        pushad
        pushfd
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_1_0188
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0188
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_0188
loc_1_016D:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 18h], ebx
        jne    SHORT loc_1_0179
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0179:
        cli
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        popfd
        pushfd
        or     eax, eax
        je     SHORT loc_1_0188
        jmp    SHORT loc_1_016D
loc_1_0188:
        popfd
        popad
        db  0FFh, 025h, 000h, 000h, 000h, 000h
loc_1_0190:
        push   eax
        push   edi
        push   1
        push   eax
        call   sub_1_00AC
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_01AA
        mov    eax, dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_1_01AA
        mov    ebx, eax
loc_1_01AA:
        pop    edi
        pop    eax
        db  0CDh, 020h, 002h, 000h, 003h, 000h
        clc
        ret
loc_1_01B4:
        push   ecx
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        pop    ecx
        clc
        ret
loc_1_01C4:
        push   edi
        push   1
        push   eax
        call   sub_1_00AC
        add    esp, 8
        or     edi, edi
        je     SHORT loc_1_01EB
        cmp    dword ptr [edi + 18h], ebx
        jne    SHORT loc_1_01EB
        jecxz  loc_1_01E3
loc_1_01DB:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_1_01F5
loc_1_01E3:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_1_01F5
loc_1_01EB:
        test   word ptr [edi + 0ch], 8000h
        jne    SHORT loc_1_01E3
        je     SHORT loc_1_01DB
loc_1_01F5:
        pop    edi
        clc
        ret
_LTEXT ends
        end
