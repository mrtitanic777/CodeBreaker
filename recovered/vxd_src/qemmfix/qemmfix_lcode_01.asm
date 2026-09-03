; qemmfix_lcode_01.asm -- QEMMFIX object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

QEMMFix_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 03Eh, 000h, 0FFh, 0FFh
        cmp    eax, 3
        je     SHORT loc_1_0019
        clc
        ret
loc_1_0019:
        db  08Bh, 035h, 08Ch, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0029
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_1_0029:
        db  08Bh, 035h, 090h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0039
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_1_0039:
        clc
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 051h, 045h, 04Dh, 04Dh
        db  046h, 069h, 078h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0094:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
