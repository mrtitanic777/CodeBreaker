; dynapage_lcode_01.asm -- DYNAPAGE object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

PAGEFILE_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
PageFile_Test_IO_Valid:
        db  080h, 03Dh, 0F4h, 000h, 000h, 000h, 002h
        jne    SHORT loc_1_002A
        db  0CDh, 020h, 008h, 000h, 015h, 000h
        jne    SHORT loc_1_002C
        push   eax
        db  0CDh, 020h, 004h, 000h, 015h, 000h
        cmp    word ptr [eax], 0
        pop    eax
        jne    SHORT loc_1_002C
loc_1_002A:
        clc
        ret
loc_1_002C:
        stc
        ret
PageFile_Read_Or_Write:
        db  080h, 03Dh, 0F5h, 000h, 000h, 000h, 000h
        db  00Fh, 085h, 03Fh, 002h, 0FFh, 0FFh
        push   eax
        push   ecx
        push   edx
        push   esi
        mov    eax, 0d600h
        mov    al, byte ptr [ebx + 4]
        xor    ecx, ecx
        mov    edx, dword ptr [ebx + 0ch]
        mov    cl, byte ptr [ebx + 7]
        shl    edx, 0ch
        shl    ecx, 0ch
        mov    esi, dword ptr [ebx + 8]
        push   ebx
        db  08Bh, 01Dh, 0F0h, 000h, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 040h, 000h
        pop    ebx
        mov    eax, 0
        adc    eax, 0
        mov    byte ptr [ebx + 6], al
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret
        db  000h, 000h, 0DFh, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Eh, 000h
        db  000h, 000h, 02Eh, 000h, 000h, 000h, 0ECh, 000h, 000h, 000h, 00Bh, 000h, 000h, 000h, 0F0h, 000h
        db  000h, 000h, 03Fh, 003h, 000h, 000h, 052h, 003h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 021h, 000h, 004h, 000h, 000h, 000h, 050h, 041h, 047h, 045h
        db  046h, 049h, 04Ch, 045h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0EDh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  078h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
