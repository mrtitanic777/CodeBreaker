; spooler_lcode_01.asm -- SPOOLER object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

SPOOLER_Control:
        sub    eax, 1
        cmp    eax, 23h
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 023h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 0B4h, 004h, 000h, 000h, 000h, 000h, 000h, 0E0h
        db  004h, 000h, 000h, 0E7h, 004h, 000h, 000h, 0ECh, 004h, 000h, 000h, 0F1h, 004h, 000h, 000h, 0F6h
        db  004h, 000h, 000h, 0FBh, 004h, 000h, 000h, 000h, 005h, 000h, 000h, 005h, 005h, 000h, 000h, 00Ah
        db  005h, 000h, 000h, 00Fh, 005h, 000h, 000h, 014h, 005h, 000h, 000h, 019h, 005h, 000h, 000h, 01Eh
        db  005h, 000h, 000h, 023h, 005h, 000h, 000h, 028h, 005h, 000h, 000h, 02Dh, 005h, 000h, 000h, 032h
        db  005h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 02Ch, 000h, 001h, 000h, 000h, 000h, 053h, 050h, 04Fh, 04Fh
        db  04Ch, 045h, 052h, 020h, 000h, 001h, 001h, 0A0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 000h, 000h, 011h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
