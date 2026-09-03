; shell_pcode_15.asm -- SHELL object 15 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_15_0000:
        db  003h, 03Dh, 004h, 00Ah, 000h, 000h
        mov    ebx, dword ptr [edi]
        or     ebx, ebx
        je     SHORT loc_15_002A
        xor    edx, edx
        mov    eax, 10009h
        lea    ecx, [edx + 1]
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        db  0E8h, 0EEh, 0FFh, 0F0h, 0FFh
        dec    ecx
        mov    dword ptr [eax - 6], ecx
        db  0E8h, 086h, 004h, 0F1h, 0FFh
loc_15_002A:
        clc
        ret
loc_15_002C:
        db  03Bh, 03Dh, 0D8h, 00Bh, 000h, 000h
        je     SHORT loc_15_0036
        clc
        ret
loc_15_0036:
        xor    eax, eax
        db  0A3h, 0D8h, 00Bh, 000h, 000h
        db  0A3h, 0B0h, 00Bh, 000h, 000h
        add    eax, 10h
        db  0BEh, 0C0h, 00Eh, 000h, 000h
        mov    ecx, 0bh
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        clc
        ret
        db  000h
_PTEXT ends
        end
