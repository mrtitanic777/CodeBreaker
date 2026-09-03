; mrci2_lcode_01.asm -- MRCI2 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

MRCI2_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        cmp    eax, 5
        db  00Fh, 084h, 0B1h, 002h, 0FFh, 0FFh
        clc
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 042h, 000h, 003h, 00Ah, 000h, 000h, 04Dh, 052h, 043h, 049h
        db  032h, 020h, 020h, 020h, 0FFh, 0FFh, 0FFh, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 07Ch, 051h, 000h, 000h, 0AFh, 061h, 000h, 000h, 0AFh, 061h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0B4h, 061h, 000h, 000h, 0EFh, 074h, 000h, 000h, 0EFh, 074h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00A8:
        dec    ebp
        push   ebx
        inc    esi
        push   esp
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        db  0E8h, 000h, 000h, 000h, 0EBh
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  080h
loc_1_00BA:
        db  0EBh, 003h
        db  000h, 000h, 000h, 000h
loc_1_00C0:
        add    dword ptr [ebx], eax
        add    eax, dword ptr [7050505h]
        pop    es
        pop    es
        pop    es
        pop    es
        pop    es
        pop    es
        pop    es
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        db  000h
loc_1_00E0:
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        push   es
        add    byte ptr [eax + eax], al
        or     al, 0
        adc    al, 0
        sbb    al, 0
        or     byte ptr [eax], al
        sbb    byte ptr [eax], al
        sub    byte ptr [eax], al
        cmp    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], bl
        push   10007800h
        add    byte ptr [eax], dh
        add    byte ptr [eax], dl
        jo     SHORT loc_1_0106
loc_1_0106:
        nop
        add    byte ptr [eax - 0fff3000h], dh
        add    byte ptr [eax], dl
        add    dword ptr [eax], esi
        add    dword ptr [eax + 1], edx
        jo     SHORT loc_1_0117
        nop
loc_1_0117:
        add    dword ptr [eax - 0ffe2fffh], esi
        add    dword ptr [eax], eax
        db  000h
loc_1_0120:
        aas
        adc    dword ptr [eax], eax
        db  000h
_LTEXT ends
        end
