; verite_lcode_01.asm -- VERITE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VERITE_Control:
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
sub_1_007D:
        mov    ecx, 0dfh
        db  08Bh, 015h, 03Ch, 001h, 000h, 000h
        add    edx, ecx
loc_1_008A:
        db  0BEh, 0A1h, 000h, 000h, 000h
        push   ecx
        push   edx
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        pop    edx
        push   edx
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        pop    edx
        pop    ecx
        dec    edx
        dec    ecx
        jge    SHORT loc_1_008A
        db  08Bh, 01Dh, 04Ch, 001h, 000h, 000h
        call   sub_1_00CD
        ret
sub_1_00B1:
        mov    ecx, 0dfh
        db  08Bh, 015h, 03Ch, 001h, 000h, 000h
        add    edx, ecx
loc_1_00BE:
        push   ecx
        push   edx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        pop    edx
        pop    ecx
        dec    edx
        dec    ecx
        jge    SHORT loc_1_00BE
        ret
sub_1_00CD:
        mov    ecx, 0dfh
        db  08Bh, 015h, 03Ch, 001h, 000h, 000h
        add    edx, ecx
loc_1_00DA:
        push   ecx
        push   edx
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        pop    edx
        pop    ecx
        dec    edx
        dec    ecx
        jge    SHORT loc_1_00DA
        ret
        db  0CCh, 0CCh, 0CCh
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 056h, 045h, 052h, 049h
        db  054h, 045h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 000h, 06Ah, 0A0h, 09Ah, 031h, 000h, 000h, 000h, 000h
        db  001h, 000h, 000h, 000h, 00Dh, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h
        db  000h, 000h, 000h, 000h, 06Ah, 0A0h, 09Ah, 031h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  010h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 050h, 000h, 000h, 034h, 027h, 034h, 029h
        db  029h, 029h, 0B1h, 0FFh, 0FFh, 0FFh, 0FFh, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h, 029h
        db  029h, 029h, 029h, 029h, 029h, 0B1h, 0B1h, 029h
_LTEXT ends
        end
