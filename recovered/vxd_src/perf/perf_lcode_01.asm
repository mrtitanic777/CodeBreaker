; perf_lcode_01.asm -- PERF object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

PERF_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 004h, 000h, 0FFh, 0FFh
        cmp    eax, 4
        db  00Fh, 084h, 0E5h, 0FFh, 0FEh, 0FFh
        cmp    eax, 24h
        db  00Fh, 084h, 0DCh, 0FFh, 0FEh, 0FFh
        cmp    eax, 3
        db  00Fh, 084h, 0DFh, 0FFh, 0FEh, 0FFh
        clc
        ret
        db  000h
sub_1_0030:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 001h, 000h, 027h, 000h
        jb     SHORT loc_1_004E
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_1_004E:
        ret    10h
        db  000h, 000h, 000h
sub_1_0054:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  000h
sub_1_0068:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0070:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0078:
        db  0CDh, 020h, 04Ah, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0080:
        db  0CDh, 020h, 04Bh, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0088:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0090:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_0098:
        db  0CDh, 020h, 052h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_00A0:
        db  0CDh, 020h, 076h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_1_00A8:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_00B0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_1_00B8:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    8
        db  000h, 000h, 000h, 09Fh, 000h, 000h, 000h, 0A6h, 000h, 000h, 000h, 05Eh, 001h, 000h, 000h, 002h
        db  002h, 000h, 000h, 0DBh, 003h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 048h, 000h, 004h, 000h, 000h, 000h, 050h, 045h, 052h, 046h
        db  020h, 020h, 020h, 020h, 000h, 000h, 090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0D0h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
