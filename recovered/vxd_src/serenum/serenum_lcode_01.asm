; serenum_lcode_01.asm -- SERENUM object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

SERENUM_Control:
        cmp    eax, 1bh
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1ch
        jne    SHORT loc_1_0017
        db  0E8h, 05Fh, 015h, 0FFh, 0FFh
        cmp    eax, 1
        ret
loc_1_0017:
        cmp    eax, 22h
        jne    SHORT loc_1_0025
        push   edx
        push   ebx
        db  0E8h, 055h, 015h, 0FFh, 0FFh
        stc
        ret
loc_1_0025:
        clc
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 004h, 000h, 000h, 000h, 053h, 045h, 052h, 045h
        db  04Eh, 055h, 04Dh, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  090h, 015h, 000h, 000h
_LTEXT ends
        end
