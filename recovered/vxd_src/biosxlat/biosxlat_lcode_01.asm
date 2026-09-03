; biosxlat_lcode_01.asm -- BIOSXLAT object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

BIOSXLAT_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
loc_1_000B:
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_1_0011:
        test   dword ptr [ebx], 40h
        je     SHORT loc_1_0043
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  066h, 03Bh, 00Dh, 098h, 000h, 000h, 000h
        jne    SHORT loc_1_0031
        db  066h, 03Bh, 015h, 09Ah, 000h, 000h, 000h
        je     SHORT loc_1_0043
loc_1_0031:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
loc_1_0043:
        stc
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 013h, 000h, 001h, 000h, 000h, 000h, 042h, 049h, 04Fh, 053h
        db  058h, 04Ch, 041h, 054h, 000h, 000h, 000h, 050h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
_LTEXT ends
        end
