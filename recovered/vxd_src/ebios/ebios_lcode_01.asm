; ebios_lcode_01.asm -- EBIOS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

EBIOS_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 3
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        cmp    eax, 7
        db  00Fh, 084h, 0E5h, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 016h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 012h, 000h, 001h, 000h, 000h, 000h, 045h, 042h, 049h, 04Fh
        db  053h, 020h, 020h, 020h, 000h, 000h, 000h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  020h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
_LTEXT ends
        end
