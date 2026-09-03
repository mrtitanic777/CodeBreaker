; isapnp_lcode_01.asm -- ISAPNP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 03Ch, 000h, 004h, 000h, 000h, 000h, 049h, 053h, 041h, 050h
        db  04Eh, 050h, 020h, 020h, 000h, 000h, 080h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
ISAPNP_Control:
        cmp    eax, 1bh
        jne    SHORT loc_1_006A
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0066:
        cmp    eax, 1
        ret
loc_1_006A:
        cmp    eax, 22h
        jne    SHORT loc_1_0078
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0076:
        stc
        ret
loc_1_0078:
        clc
        ret
        db  0CCh, 0CCh
_LTEXT ends
        end
