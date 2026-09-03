; mmdevldr_lcode_01.asm -- MMDEVLDR object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 04Ah, 004h, 001h, 000h, 000h, 000h, 06Dh, 06Dh, 064h, 065h
        db  076h, 06Ch, 064h, 072h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
mmdevldr_Control:
        db  083h, 0F8h, 023h, 00Fh, 084h, 000h, 000h, 000h, 000h, 083h, 0F8h, 01Bh, 00Fh, 084h, 000h, 000h
        db  000h, 000h, 083h, 0F8h, 01Ch, 00Fh, 084h, 000h, 000h, 000h, 000h, 083h, 0F8h, 022h, 00Fh, 084h
        db  000h, 000h, 000h, 000h, 0F8h, 0C3h, 0CCh, 0CCh
_LTEXT ends
        end
