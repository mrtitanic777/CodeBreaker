; vcond_lcode_01.asm -- VCOND object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VCOND_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 19h
        je     SHORT loc_1_0017
        clc
        ret
VCOND_Get_Version:
        mov    eax, 100h
        clc
        ret
loc_1_0017:
        push   ebx
        db  0E8h, 045h, 01Dh, 0FFh, 0FFh
        clc
        ret
        db  000h
sub_1_0020:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0028:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0030:
        pop    edx
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        jmp    edx
        db  000h, 000h, 010h, 000h, 000h, 000h, 09Fh, 001h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 038h, 000h, 001h, 000h, 000h, 000h, 056h, 043h, 04Fh, 04Eh
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  02Ah, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  03Ch, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
