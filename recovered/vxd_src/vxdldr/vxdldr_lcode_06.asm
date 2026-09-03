; vxdldr_lcode_06.asm -- VXDLDR object 6 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_6_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edi], edx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    dword ptr [eax], eax
        adc    al, 20h
        add    byte ptr [eax], al
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        je     6
        add    byte ptr [eax], al
        pushad
        and    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        xor    byte ptr [eax], ah
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        push   esp
        out    0, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  074h, 0E7h
        add    byte ptr [eax], al
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        adc    al, 0e7h
        add    byte ptr [eax], al
        add    al, 22h
        add    byte ptr [eax], al
        add    eax, 34000000h
        out    0, eax
        add    byte ptr [eax], ah
        and    al, byte ptr [eax]
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax + eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax + eax], dl
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 054h, 0E7h, 000h, 000h, 044h, 0A0h, 000h, 000h, 008h, 000h, 000h, 000h
        db  014h, 0E7h, 000h, 000h, 014h, 0A0h, 000h, 000h, 014h, 000h, 000h, 000h, 054h, 0E7h, 000h, 000h
        db  004h, 0A0h, 000h, 000h, 009h, 000h, 000h, 000h, 054h, 0E7h, 000h, 000h, 044h, 060h, 000h, 000h
        db  00Bh, 000h, 000h, 000h, 074h, 0E7h, 000h, 000h, 060h, 060h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  000h, 000h, 008h, 005h, 002h, 00Bh, 055h, 00Bh, 005h, 002h, 005h, 008h, 001h, 000h, 000h, 038h
        db  002h, 000h, 000h, 040h, 002h, 000h, 000h, 057h, 002h, 000h, 000h, 0BEh, 002h, 000h, 000h, 01Bh
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Eh
        db  064h, 065h, 062h, 075h, 067h, 000h, 000h, 049h, 04Eh, 049h, 054h, 000h, 000h, 000h, 000h, 02Eh
        db  064h, 065h, 062h, 075h, 067h, 000h, 000h, 02Eh, 064h, 065h, 062h
_LTEXT ends
        end
