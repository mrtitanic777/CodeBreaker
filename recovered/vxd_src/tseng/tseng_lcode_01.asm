; tseng_lcode_01.asm -- TSENG object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

TSENG_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 0EAh, 001h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h
        db  00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_007D:
        db  0F6h, 005h, 061h, 001h, 000h, 000h, 003h
        jne    SHORT loc_1_00D5
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        cmp    ebx, edi
        jne    SHORT loc_1_00B9
        db  03Bh, 01Dh, 044h, 001h, 000h, 000h
        jne    SHORT loc_1_00D5
        and    edx, 0ffffh
        sub    edx, 3cbh
        shr    edx, 1
        db  081h, 0C2h, 065h, 001h, 000h, 000h
        cmp    ecx, 0
        je     SHORT loc_1_00B5
        mov    byte ptr [edx], al
        jmp    SHORT loc_1_00F3
loc_1_00B5:
        mov    al, byte ptr [edx]
        jmp    SHORT loc_1_00F3
loc_1_00B9:
        cmp    ecx, 4
        je     SHORT loc_1_00DB
        and    edx, 0ffffh
        sub    edx, 3cbh
        db  003h, 015h, 058h, 001h, 000h, 000h
        mov    al, byte ptr [ebx + edx]
        jmp    SHORT loc_1_00F3
loc_1_00D5:
        cmp    ecx, 0
        je     SHORT loc_1_00F2
        out    dx, al
loc_1_00DB:
        and    edx, 0ffffh
        sub    edx, 3cbh
        db  003h, 015h, 058h, 001h, 000h, 000h
        mov    byte ptr [ebx + edx], al
        jmp    SHORT loc_1_00F3
loc_1_00F2:
        in     al, dx
loc_1_00F3:
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 001h, 000h, 000h, 054h, 053h, 045h, 04Eh
        db  047h, 020h, 020h, 020h, 000h, 000h, 000h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
