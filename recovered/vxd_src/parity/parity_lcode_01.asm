; parity_lcode_01.asm -- PARITY object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

PARITY_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
loc_1_000B:
        push   eax
        in     al, 61h
        test   al, 80h
        je     SHORT loc_1_0026
        jmp    SHORT loc_1_0014
loc_1_0014:
        or     al, 8
        out    61h, al
        jmp    SHORT loc_1_001A
loc_1_001A:
        and    al, 0f7h
        out    61h, al
        jmp    SHORT loc_1_0020
loc_1_0020:
        in     al, 61h
        test   al, 80h
        jne    SHORT loc_1_007C
loc_1_0026:
        db  0F7h, 005h, 014h, 001h, 000h, 000h, 000h, 000h, 004h, 000h
        je     SHORT loc_1_0053
        jmp    SHORT loc_1_0034
loc_1_0034:
        in     al, 90h
        test   al, 20h
        je     SHORT loc_1_0049
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_0089
loc_1_0049:
        test   al, 40h
        je     SHORT loc_1_0053
        and    al, 0bfh
        out    90h, al
        jmp    SHORT loc_1_0053
loc_1_0053:
        mov    al, 80h
        out    70h, al
        jmp    SHORT loc_1_0059
loc_1_0059:
        in     al, 71h
        jmp    SHORT loc_1_005D
loc_1_005D:
        in     al, 61h
        jmp    SHORT loc_1_0061
loc_1_0061:
        or     al, 0ch
        out    61h, al
        jmp    SHORT loc_1_0067
loc_1_0067:
        and    al, 0f3h
        out    61h, al
        jmp    SHORT loc_1_006D
loc_1_006D:
        mov    al, 0dh
        out    70h, al
        jmp    SHORT loc_1_0073
loc_1_0073:
        in     al, 71h
        pop    eax
        db  0FFh, 025h, 010h, 001h, 000h, 000h
loc_1_007C:
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
loc_1_0089:
        db  0F7h, 005h, 014h, 001h, 000h, 000h, 000h, 000h, 004h, 000h
        je     SHORT loc_1_009D
        in     al, 90h
        and    al, 0bfh
        jmp    SHORT loc_1_009B
loc_1_009B:
        out    90h, al
loc_1_009D:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 0e0001010h
        db  00Fh, 0B7h, 03Dh, 010h, 000h, 000h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        cli
loc_1_00BC:
        jmp    SHORT loc_1_00BC
        db  000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 008h, 000h, 001h, 000h, 000h, 000h, 050h, 041h, 052h, 049h
        db  054h, 059h, 020h, 020h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
