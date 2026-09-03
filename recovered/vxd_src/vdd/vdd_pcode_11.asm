; vdd_pcode_11.asm -- VDD object 11 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_11_0000:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_11_0054
        call   sub_11_0094
        db  0FFh, 00Dh, 094h, 007h, 000h, 000h
        jne    SHORT loc_11_0034
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 38h], 0
        je     SHORT loc_11_0029
        call   dword ptr [edi + 38h]
loc_11_0029:
        pop    edi
        db  081h, 025h, 09Ch, 010h, 000h, 000h, 0FFh, 0FFh, 0FFh, 07Fh
loc_11_0034:
        clc
        ret
loc_11_0036:
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_11_0052
        db  0C7h, 005h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h
loc_11_0052:
        clc
        ret
sub_11_0054:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_11_006B
        db  0E8h, 0A1h, 017h, 0F5h, 0FFh
        db  0C7h, 005h, 0A8h, 007h, 000h, 000h, 000h, 000h, 000h, 000h
loc_11_006B:
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_11_0093
        push   ebx
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_11_008D
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 013h, 011h, 0F5h, 0FFh
        jmp    SHORT loc_11_0092
loc_11_008D:
        db  0E8h, 028h, 018h, 0F5h, 0FFh
loc_11_0092:
        pop    ebx
loc_11_0093:
        ret
sub_11_0094:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        and    byte ptr [edi + 5e2h], 7fh
        db  0E8h, 090h, 044h, 0F5h, 0FFh
        db  0E8h, 018h, 036h, 0F5h, 0FFh
        db  0E8h, 0FFh, 000h, 0F5h, 0FFh
        mov    eax, dword ptr [edi + 5b8h]
        or     eax, eax
        je     SHORT loc_11_00C8
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_11_00C8:
        mov    eax, dword ptr [edi + 5bch]
        or     eax, eax
        je     SHORT loc_11_00DE
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_11_00DE:
        ret
        db  000h
_PTEXT ends
        end
