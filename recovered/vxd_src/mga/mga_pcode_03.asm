; mga_pcode_03.asm -- MGA object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 08Ch, 00Bh, 000h, 000h
        jne    SHORT loc_3_001B
        cmp    ebx, edi
        je     SHORT loc_3_001B
        db  0A1h, 090h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_003D
loc_3_001B:
        cmp    ecx, 4
        je     SHORT loc_3_0033
        cmp    ecx, 0
        je     SHORT loc_3_003A
        cmp    ecx, 0ch
        je     SHORT loc_3_002F
        cmp    ecx, 8
        je     SHORT loc_3_0036
loc_3_002F:
        out    dx, ax
        jmp    SHORT loc_3_003D
loc_3_0033:
        out    dx, al
        jmp    SHORT loc_3_003D
loc_3_0036:
        in     ax, dx
        jmp    SHORT loc_3_003D
loc_3_003A:
        in     al, dx
        jmp    SHORT loc_3_003D
loc_3_003D:
        pop    esi
        pop    edi
        ret
_PTEXT ends
        end
