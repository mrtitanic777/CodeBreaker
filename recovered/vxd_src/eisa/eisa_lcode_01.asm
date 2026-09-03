; eisa_lcode_01.asm -- EISA object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

EISA_Control:
        cmp    eax, 1
        jne    SHORT loc_1_0013
        db  068h, 034h, 000h, 000h, 000h
        db  0E8h, 001h, 00Eh, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_0013:
        cmp    eax, 1bh
        jne    SHORT loc_1_0023
        push   0
        db  0E8h, 0F1h, 00Dh, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_0023:
        cmp    eax, 22h
        jne    SHORT loc_1_0031
        push   edx
        push   ebx
        db  0E8h, 001h, 00Eh, 0FFh, 0FFh
        stc
        ret
loc_1_0031:
        clc
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 045h, 049h, 053h, 041h
        db  020h, 020h, 020h, 020h, 000h, 000h, 080h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
