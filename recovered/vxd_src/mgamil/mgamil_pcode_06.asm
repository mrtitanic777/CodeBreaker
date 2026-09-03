; mgamil_pcode_06.asm -- MGAMIL object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

MGAMIL_SYS_VM_TERMINATE:
        db  0E8h, 078h, 000h, 0FAh, 0FFh
        clc
        ret
loc_6_0007:
        db  0E8h, 071h, 000h, 0FAh, 0FFh
        ret
loc_6_000D:
        ret
loc_6_000E:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 02Ch, 008h, 000h, 000h
        jne    NEAR PTR loc_6_00EC
        cmp    ebx, edi
        je     NEAR PTR loc_6_00EC
        cmp    ecx, 4
        je     SHORT loc_6_0073
        cmp    ecx, 0
        je     NEAR PTR loc_6_00D5
        cmp    ecx, 0ch
        je     SHORT loc_6_0042
        cmp    ecx, 8
        je     SHORT loc_6_00AF
loc_6_0042:
        cmp    dx, 3deh
        jne    NEAR PTR loc_6_010E
        db  0C7h, 005h, 050h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    al, 7
        jne    NEAR PTR loc_6_010E
        db  0A2h, 04Ch, 008h, 000h, 000h
        db  0C7h, 005h, 050h, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    NEAR PTR loc_6_010E
loc_6_0073:
        cmp    dx, 3deh
        jne    SHORT loc_6_0098
        db  0C7h, 005h, 050h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    al, 7
        jne    NEAR PTR loc_6_010E
        db  0C7h, 005h, 050h, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_6_010E
loc_6_0098:
        cmp    dx, 3dfh
        jne    SHORT loc_6_010E
        db  083h, 03Dh, 050h, 008h, 000h, 000h, 001h
        jne    SHORT loc_6_010E
        db  0A2h, 04Ch, 008h, 000h, 000h
        jmp    SHORT loc_6_010E
loc_6_00AF:
        cmp    dx, 3deh
        jne    SHORT loc_6_010E
        db  0C7h, 005h, 050h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        cmp    al, 7
        jne    SHORT loc_6_010E
        db  0A0h, 04Ch, 008h, 000h, 000h
        db  0C7h, 005h, 050h, 008h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_6_010E
loc_6_00D5:
        cmp    dx, 3dfh
        jne    SHORT loc_6_010E
        db  083h, 03Dh, 050h, 008h, 000h, 000h, 001h
        jne    SHORT loc_6_010E
        db  0A0h, 04Ch, 008h, 000h, 000h
        jmp    SHORT loc_6_010E
loc_6_00EC:
        cmp    ecx, 4
        je     SHORT loc_6_0104
        cmp    ecx, 0
        je     SHORT loc_6_010B
        cmp    ecx, 0ch
        je     SHORT loc_6_0100
        cmp    ecx, 8
        je     SHORT loc_6_0107
loc_6_0100:
        out    dx, ax
        jmp    SHORT loc_6_010E
loc_6_0104:
        out    dx, al
        jmp    SHORT loc_6_010E
loc_6_0107:
        in     ax, dx
        jmp    SHORT loc_6_010E
loc_6_010B:
        in     al, dx
        jmp    SHORT loc_6_010E
loc_6_010E:
        pop    esi
        pop    edi
        ret
loc_6_0111:
        db  066h, 08Bh, 00Dh, 056h, 008h, 000h, 000h
        shl    ecx, 10h
        stc
        ret
        db  024h, 04Dh, 047h
_PTEXT ends
        end
