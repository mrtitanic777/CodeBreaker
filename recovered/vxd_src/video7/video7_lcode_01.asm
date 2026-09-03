; video7_lcode_01.asm -- VIDEO7 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VIDEO7_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_007D:
        db  08Bh, 00Dh, 06Ah, 001h, 000h, 000h
        stc
        ret
        db  000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 056h, 049h, 044h, 045h
        db  04Fh, 037h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0172:
        inc    dword ptr [eax]
loc_1_0174:
        add    byte ptr [eax], al
loc_1_0176:
        add    byte ptr [eax], al
loc_1_0178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
