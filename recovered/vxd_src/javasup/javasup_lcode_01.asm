; javasup_lcode_01.asm -- JAVASUP object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 04Ah, 041h, 056h, 041h
        db  053h, 055h, 050h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
JAVASUP_Control:
        db  083h, 0F8h, 031h, 00Fh, 084h, 000h, 000h, 000h, 000h, 083h, 0F8h, 000h, 00Fh, 084h, 000h, 000h
        db  000h, 000h, 0F8h, 0C3h
_LTEXT ends
        end
