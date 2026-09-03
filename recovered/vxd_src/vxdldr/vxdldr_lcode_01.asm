; vxdldr_lcode_01.asm -- VXDLDR object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VXDLDR_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 032h, 000h, 0FFh, 0FFh
        cmp    eax, 23h
        db  00Fh, 084h, 052h, 002h, 0FFh, 0FFh
        cmp    eax, 3
        db  00Fh, 084h, 0DCh, 0FFh, 0FEh, 0FFh
        db  0F7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_1_0039
        cmp    eax, 14h
        db  00Fh, 084h, 0C7h, 0FFh, 0FEh, 0FFh
loc_1_0039:
        clc
        ret
VXDLDR_GetVersion:
        mov    eax, 100h
        clc
        ret
VXDLDR_GetDeviceList:
        db  0A1h, 0E0h, 000h, 000h, 000h
        ret
        db  03Bh, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 0BDh, 000h, 000h, 000h, 0D2h, 000h, 000h, 000h
        db  046h, 001h, 000h, 000h, 042h, 000h, 000h, 000h, 098h, 001h, 000h, 000h, 090h, 002h, 000h, 000h
        db  0B0h, 00Ah, 000h, 000h, 000h, 00Bh, 000h, 000h, 030h, 00Bh, 000h, 000h, 060h, 00Bh, 000h, 000h
        db  080h, 00Ch, 000h, 000h, 060h, 00Dh, 000h, 000h, 020h, 00Eh, 000h, 000h, 0DEh, 00Fh, 000h, 000h
        db  040h, 00Fh, 000h, 000h, 0B0h, 002h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 027h, 000h, 003h, 000h, 000h, 000h, 056h, 058h, 044h, 04Ch
        db  044h, 052h, 020h, 020h, 000h, 000h, 000h, 016h, 000h, 000h, 000h, 000h, 00Ah, 002h, 000h, 000h
        db  00Ah, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  048h, 000h, 000h, 000h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
